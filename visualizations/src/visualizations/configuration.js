function ConfigurationView(data, elementSel, configuration, events) {
    var self = this;
    this.render = function() {
        $(elementSel).html("");
        self.setDimension();
        self.renderDefualtVis();
        self.renderAnimationTime();
        self.renderZoomAnimationTime();
        self.renderGradiante();
        self.renderFilterNodes();
        self.renderFilterProbabilty();
        self.renderRegexFilter();
        self.renderDefualtTableEntries();
        self.renderChosenScript();
        this.renderKeyBindings();
        this.renderResetButton();
    };

    this.setDimension = function(){
        $(elementSel).css( "overflow-y","scroll");
        $(elementSel).css( "height",$(window).height()-55);
    }

    this.renderDefualtVis = function() {
        $(elementSel).append('<p><label for="defaultVis">Select the default visualization:</label></p><select name="defaultVis" id="defaultVis"></select>');
        $.each(visualizations, function(index, visualization) {
            $("#defaultVis").append('<option id="defaultVis_' + index + '" value="' + index + '">' + visualization.displayName + '</option>');
        });
        $("#defaultVis").append('<option id="defaultVis_last_viewed" value="last_viewed">Last Viewed</option>');
        $('#defaultVis_' + configuration.currentConfig.defaultView).attr("selected", "selected");
        $("#defaultVis").selectmenu({
            change: function(event, data) {
                configuration.currentConfig.defaultView = data.item.value;
                configuration.saveConfig();
            }
        });
    };

    function defaultRenderValue(value) {
        return value;
    }

    function zeroDisableRenderValue(value) {
        if (value > 0) {
            return value;
        }
        return "Disabled";
    }

    function sliderRender(element, id, initValue, maxValue, renderValue, updateValue) {
        $(element).append('<input type="text" id="input_' + id + '" readonly style="border:0; color:#f6931f; font-weight:bold;"><br /><br /><div id="slider_' + id + '"></div>');
        $('#slider_' + id).slider({
            range: "min",
            value: initValue,
            min: 0,
            max: Math.max(initValue * 2, maxValue),
            slide: function(event, ui) {
                $('#input_' + id).val(renderValue(ui.value));
                updateValue(ui.value);
            }
        });
        $('#input_' + id).val(renderValue(initValue));
    }

    this.renderAnimationTime = function() {
        $(elementSel).append('<br /><br /><label for="input_aniTime">Animation time (ms):</label>');
        sliderRender(elementSel, 'aniTime', configuration.currentConfig.animationTransitionTime, 5000, defaultRenderValue, configuration.saveAnimationTime);
    };

    this.renderZoomAnimationTime = function() {
        $(elementSel).append('<br /><br /><label for="zoomAniTime">Zoom animation time (ms):</label>');
        sliderRender(elementSel, 'zoomAniTime', configuration.currentConfig.zoomAnimationTime, 5000, defaultRenderValue, configuration.saveZoomAnimationTime);
    };



    this.renderFilterNodes = function() {
        function filterUpdated(value) {
            configuration.saveMostRelevantFilter(value);
            events.filtersUpdate();
        }

        $(elementSel).append('<br /><br /><label>Filter to most relevant nodes:</label>');
        sliderRender(elementSel, 'filterMRelevant', configuration.currentConfig.filterMostRelevamtNodes, 500, zeroDisableRenderValue, filterUpdated);
    };



    this.renderFilterProbabilty = function() {
        function filterUpdated(value) {
            configuration.saveProbabilityFilter(value);
            events.filtersUpdate();
        }

        $(elementSel).append('<br /><br /><label>Filter by probability:</label>');
        sliderRender(elementSel, 'filterProbabilty', configuration.currentConfig.filterMinProbability, 100, zeroDisableRenderValue, filterUpdated);
    };

    this.renderGradiante = function() {

        function convertGradStep(gradStep) {
            return {
                color: gradStep[0],
                position: gradStep[1]
            };
        }

        function convertGradRepresentation(grad) {
            ret = [];
            for (var i = grad.length - 1; i >= 0; i--) {
                ret[i] = convertGradStep(grad[i]);
            }
            return ret;
        }


        $(elementSel).append('<br /><label>Gradient: </label><div id="gradX" ></div>');

        gradX("#gradX", {
            type: "linear",
            change: function(sliders, styles) {
                configuration.currentConfig.normalGradiante = convertGradRepresentation(sliders);
                configuration.saveGradiante();
            },
            sliders: configuration.currentConfig.normalGradiante
        });

        $("#gradx_gradient_type, #gradx_gradient_subtype, #gradx_show_code").remove();
    };

    this.renderRegexFilter = function() {
        $(elementSel).append('<br /><br/><div class="ui-widget"><label for="regex">Regular Expression to filter nodes: </label><br /><input id="regex" size="30"></div>');
        $("#regex").val(configuration.currentConfig.regexFilter);
        $("#regex").change(function() {
            configuration.currentConfig.regexFilter = $("#regex").val();
            configuration.saveConfig();
            events.filtersUpdate();
        });
    };


    this.renderDefualtTableEntries = function() {
        $(elementSel).append('<p><label for="defaultTE">Default number of table entries:</label></p><select style="width: 100px;" name="defaultTE" id="defaultTE"></select>');
        $("#defaultTE").append('<option value="10">10</option>');
        $("#defaultTE").append('<option value="25">25</option>');
        $("#defaultTE").append('<option value="50">50</option>');
        $("#defaultTE").append('<option value="100">100</option>');
        $('option[value="' + configuration.currentConfig.defaultTableEntries + '"]', "#defaultTE").attr("selected", "selected");
        $("#defaultTE").selectmenu({
            change: function(event, data) {
                configuration.currentConfig.defaultTableEntries = data.item.value;
                configuration.saveConfig();
            }
        });
    };

    this.renderResetButton = function() {
        $(elementSel).append('<br /><button id="buttonReset">Reset to default</button>');
        $("#buttonReset").button()
            .click(function(event) {
                configuration.resetConfig();
                self.render();
            });
    };

    this.renderChosenScript = function() {
        $(elementSel).append('<br /><br/><div class="ui-widget"><label for="tags">Additional scripts to load: </label><br /><input id="tags" size="50"></div>');

        function split(val) {
            return val.split(/,\s*/);
        }

        function extractLast(term) {
            return split(term).pop();
        }
        $("#tags")
            // don't navigate away from the field on tab when selecting an item
            .bind("keydown", function(event) {
                if (event.keyCode === $.ui.keyCode.TAB &&
                    $(this).autocomplete("instance").menu.active) {
                    event.preventDefault();
                }
            })
            .autocomplete({
                minLength: 0,
                source: function(request, response) {
                    // delegate back to autocomplete, but extract the last term
                    response($.ui.autocomplete.filter(
                        configuration.currentConfig.scriptsLoadAutoComplete, extractLast(request.term)));
                },
                create: function() {
                    var finalTerm = [];
                    $.each(configuration.currentConfig.scriptsLoad, function(index, term) {
                        if (term.length > 3 && !(finalTerm.indexOf(term) >= 0) && urlExists(term.trim())) {
                            finalTerm.push(term.trim());
                        }
                    });
                    configuration.currentConfig.scriptsLoad = finalTerm;
                    configuration.saveConfig();
                    this.value = configuration.currentConfig.scriptsLoad.join(", ");
                },
                focus: function() {
                    // prevent value inserted on focus
                    return false;
                },
                change: function(event, ui) {
                    var terms = split(this.value);
                    var finalTerm = [];
                    $.each(terms, function(index, term) {
                        if (term.length > 3 && !(finalTerm.indexOf(term) >= 0) && urlExists(term.trim())) {
                            finalTerm.push(term.trim());
                        }
                    });
                    finalTerm = jQuery.unique(finalTerm);
                    configuration.currentConfig.scriptsLoad = finalTerm;
                    configuration.currentConfig.scriptsLoadAutoComplete = configuration.currentConfig.scriptsLoadAutoComplete.concat(finalTerm).unique();
                    configuration.saveConfig();
                },
                select: function(event, ui) {
                    var terms = split(this.value);
                    // remove the current input
                    terms.pop();
                    // add the selected item
                    terms.push(ui.item.value);
                    // add placeholder to get the comma-and-space at the end
                    terms.push("");
                    this.value = terms.join(", ");
                    return false;
                }
            });
    };

    this.renderKeyBindings = function() {

        var keyBindingToChange = null;
        var pressedKeys = [];

        function getKeyBindingById(id) {
            var tmp = id.replace('mode_', '').split('_');
            if (tmp[0] >= 0) {
                return configuration.currentConfig.keyBindings.modes[tmp[0]].keyBindings[tmp[1]];
            }
            return configuration.currentConfig.keyBindings.allwaysActive[tmp[1]];
        }

        function changeClicked() {
            keyBindingToChange = getKeyBindingById($(this).attr('id'));
            pressedKeys = [];
            $('.pressed_keys').html('');
            $("#dialogKeyChange").dialog("open");
            document.onkeydown = function(e) {
                pressedKeys.push(e.keyCode);
                $('.pressed_keys').html(renderKeyCodesArray(pressedKeys));
            };
            return false;
        }

        function dialogOkButtonClicked() {
            if (keyBindingToChange !== null && pressedKeys.length > 0) {
                keyBindingToChange.keyCodes = pressedKeys;
                $("#dialogKeyChange").dialog("close");
                configuration.saveConfig();
                renderConfigDisplay();
            }
        }

        function dialogInit() {
            $("#dialogKeyChange").remove();
            $(elementSel).append('<div id="dialogKeyChange" title="Press the keys, then click ok!"><b>Selected Key(s):</b><br /> <span class="pressed_keys"></span></div>');
            $("#dialogKeyChange").dialog({
                autoOpen: false,
                modal: true,
                show: {
                    effect: "blind",
                    duration: 1000
                },
                hide: {
                    effect: "explode",
                    duration: 1000
                },
                buttons: {
                    "Ok": dialogOkButtonClicked
                },
            });
        }

        function renderConfigDisplay(){
            $('.keyConfig').remove();
            $(elementSel).append('<div class="keyConfig"></div>');
            $('.keyConfig').html('<p class="keyBindingsp">Key Bindings:</p>' + renderKeyBindingsDisplay(configuration,true));
            $('.keyChange').on("click", changeClicked);
        }

        renderConfigDisplay();
        dialogInit();
    };
}


Array.prototype.unique = function() {
    var tmp = {},
        out = [];
    for (var i = 0, n = this.length; i < n; ++i) {
        if (!tmp[this[i]]) {
            tmp[this[i]] = true;
            out.push(this[i]);
        }
    }
    return out;
};

function urlExists(testUrl) {
    var http = jQuery.ajax({
        type: "HEAD",
        url: testUrl,
        async: false
    })
    return http.status == 200;
}