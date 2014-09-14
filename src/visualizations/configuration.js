function ConfigurationView(data, elementSel, configuration, events) {
    var self = this;
    this.render = function() {
        $(elementSel).html("");
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
    }

    this.renderAnimationTime = function() {
        $(elementSel).append('<p><label for="anitime">Animation time (ms):</label><input type="text" id="anitime" readonly style="border:0; color:#f6931f; font-weight:bold;"></p><div id="slider-anitime"></div>');

        $("#slider-anitime").slider({
            range: "min",
            value: configuration.currentConfig.animationTransitionTime,
            min: 0,
            max: Math.max(configuration.currentConfig.animationTransitionTime * 2, 5000),
            slide: function(event, ui) {
                $("#anitime").val(ui.value);
                configuration.currentConfig.animationTransitionTime = ui.value;
                configuration.saveConfig();
            }
        });
        $("#anitime").val($("#slider-anitime").slider("value"));
    }

    this.renderZoomAnimationTime = function() {
        $(elementSel).append('<p><label for="zanitime">Zoom animation time (ms):</label><input type="text" id="zanitime" readonly style="border:0; color:#f6931f; font-weight:bold;"></p><div id="slider-zanitime"></div>');

        $("#slider-zanitime").slider({
            range: "min",
            value: configuration.currentConfig.zoomAnimationTime,
            min: 0,
            max: Math.max(configuration.currentConfig.zoomAnimationTime * 2, 5000),
            slide: function(event, ui) {
                $("#zanitime").val(ui.value);
                configuration.currentConfig.zoomAnimationTime = ui.value;
                configuration.saveConfig();
            }
        });
        $("#zanitime").val($("#slider-zanitime").slider("value"));
    }



    this.renderFilterNodes = function() {
        $(elementSel).append('<p><label for="nnodes">Filter to most relevant nodes:</label><input type="text" id="nnodes" readonly style="border:0; color:#f6931f; font-weight:bold;"></p><div id="slider-nnodes"></div>');

        function renderValue(value) {
            if (value > 0) {
                return value;
            }
            return "Disabled";
        }

        $("#slider-nnodes").slider({
            range: "min",
            value: configuration.currentConfig.filterMostRelevamtNodes,
            min: 0,
            max: Math.max(configuration.currentConfig.filterMostRelevamtNodes * 2, 500),
            slide: function(event, ui) {
                $("#nnodes").val(renderValue(ui.value));
                configuration.currentConfig.filterMostRelevamtNodes = ui.value;
                configuration.saveConfig();
                events.filtersUpdate();
            }
        });
        $("#nnodes").val(renderValue(configuration.currentConfig.filterMostRelevamtNodes));
    }



    this.renderFilterProbabilty = function() {
        $(elementSel).append('<p><label for="pnodes">Filter by probability:</label><input type="text" id="pnodes" readonly style="border:0; color:#f6931f; font-weight:bold;"></p><div id="slider-pnodes"></div>');

        function renderValue(value) {
            if (value > 0) {
                return value;
            }
            return "Disabled";
        }

        $("#slider-pnodes").slider({
            range: "min",
            value: configuration.currentConfig.filterMinProbability,
            min: 0,
            max: 100,
            slide: function(event, ui) {
                $("#pnodes").val(renderValue(ui.value));
                configuration.currentConfig.filterMinProbability = ui.value;
                configuration.saveConfig();
                events.filtersUpdate();
            }
        });
        $("#pnodes").val(renderValue(configuration.currentConfig.filterMinProbability));
    }

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
            };
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
    }

    this.renderRegexFilter = function() {
        $(elementSel).append('<br /><br/><div class="ui-widget"><label for="regex">Regular Expression to filter nodes: </label><br /><input id="regex" size="50"></div>');
        $("#regex").val(configuration.currentConfig.regexFilter);
        $("#regex").change(function() {
            configuration.currentConfig.regexFilter = $("#regex").val();
            configuration.saveConfig();
            events.filtersUpdate();
        });
    }


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
    }

    this.renderResetButton = function() {
        $(elementSel).append('<br /><button id="buttonReset">Reset to default</button>');
        $("#buttonReset").button()
        .click(function(event) {
            configuration.resetConfig();
            self.render();
        });
    }

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
}

this.renderKeyBindings = function() {
    var keyBindingChange = null;
    var pressedKeys = [];


    function renderKeyCodesArray(keyCodes) {
        var renderStr = '';
        for (var i = 0; i < keyCodes.length; i++) {
            renderStr += KeyboardJS.key.name(keyCodes[i])[0] + ' ';
        };
        return renderStr;
    }

    function keyBindingsRender(arrayKeyBindings, modeID) {
        var renderStr = '';
        for (var i = 0; i < arrayKeyBindings.length; i++) {
            renderStr += '<p><small>' + arrayKeyBindings[i].name + ': ';
            renderStr += renderKeyCodesArray(arrayKeyBindings[i].keyCodes);
            renderStr += ' <a id="mode_' + modeID + '_' + i + '" href="#" class="keyChange">Change</a></small></p>';
        };
        return renderStr;
    }

    function modesRender(modes) {
        var str = '';
        for (var i = 0; i < modes.length; i++) {
            str += '<br /><small>' + modes[i].name + ' Mode</small>';
            str += keyBindingsRender(modes[i].keyBindings, i);
        };
        return str;
    }

    function getKeyBindingById(id) {
        var tmp = id.replace('mode_', '').split('_');
        if (tmp[0] >= 0) {
            return configuration.currentConfig.keyBindings.modes[tmp[0]].keyBindings[tmp[1]];
        }
        return configuration.currentConfig.keyBindings.allwaysActive[tmp[1]];
    }

    function changeClicked() {
        keyBindingChange = getKeyBindingById($(this).attr('id'));
        pressedKeys = [];
        $('.pressed_keys').html('');
        $("#dialog").dialog("open");
        document.onkeydown = function(e) {
            pressedKeys.push(e.keyCode);
            $('.pressed_keys').html(renderKeyCodesArray(pressedKeys));
        }
        return false;
    }

    function renderConfigDisplay() {
        $('.keyConfig').html('<p>Key Bindings:</p>' +
            keyBindingsRender(configuration.currentConfig.keyBindings.allwaysActive, -1) +
            modesRender(configuration.currentConfig.keyBindings.modes));
        $('.keyChange').on("click", changeClicked);
    }




    $(elementSel).append('<div class="keyConfig"></div>');
    renderConfigDisplay();



    $("#dialog").remove();
    $(elementSel).append('<div id="dialog" title="Press the keys, then click ok!"><b>Selected Key(s):</b><br /> <span class="pressed_keys"></span></div>');
    $("#dialog").dialog({
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
            "Ok": function() {
                if (keyBindingChange != null && pressedKeys.length > 0) {
                    keyBindingChange.keyCodes = pressedKeys;
                    $("#dialog").dialog("close");
                    configuration.saveConfig();
                    renderConfigDisplay();
                }
            }
        },
    });




}
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
}

function urlExists(testUrl) {
    var http = jQuery.ajax({
        type: "HEAD",
        url: testUrl,
        async: false
    })
    return http.status == 200;
}