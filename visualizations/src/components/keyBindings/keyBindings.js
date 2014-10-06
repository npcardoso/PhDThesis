function keyBindings(visualization, configuration) {
    var zoomEvents = visualization.zoomEvents;
    var currentKeyBindings = configuration.currentConfig.keyBindings.allwaysActive;
    var pressedKeys = [];
    var currentMode = null;

    function findByName(name, array) {
        for (var i = array.length - 1; i >= 0; i--) {
            if (array[i].name == name) {
                return array[i];
            }
        }
        return null;
    }

    function getKeyBindingByName(name) {
        return findByName(name, currentKeyBindings);
    }

    function getModeByName(name) {
        return findByName(name, configuration.currentConfig.keyBindings.modes);
    }


    function getKeyBindingByKeyCodes(keyCodes) {
        for (var i = currentKeyBindings.length - 1; i >= 0; i--) {
            if (keyMatch(keyCodes, currentKeyBindings[i].keyCodes)) {
                return currentKeyBindings[i];
            }
        }
        return null;
    }

    function setKeyBinding(name, func) {
        getKeyBindingByName(name).func = func;
    }

    function setModeKeyBinding(mode, keyBindingName, func) {
        var keyBinding = findByName(keyBindingName, mode.keyBindings);
        keyBinding.func = func;
    }

    function initZoomMode() {
        zoomMode = getModeByName("Zoom");
        setModeKeyBinding(zoomMode, "Zoom In", zoomEvents.zoomIn);
        setModeKeyBinding(zoomMode, "Zoom Out", zoomEvents.zoomOut);
        setModeKeyBinding(zoomMode, "Zoom Reset", zoomEvents.zoomReset);
        zoomMode.exitHandler = function() {
            $('#zoomContainer').hide();
        };
    }

    function initFilteringMode() {
        function dialogRender(){
            sel = $('<div title="Filter"></div>');
            $('body').append(sel);
            sel.dialog({
                autoOpen: true,
                modal: true,
            close: function(event, ui) 
            { 
                $(this).dialog('close');
                $(this).remove();
            } 
            });
            return sel;
        }

        filteringMode = getModeByName("Filtering");

        setModeKeyBinding(filteringMode, "Most Relevant", function(){
            var sel = dialogRender();
            var conf_view = new ConfigurationView(null,sel,configuration,visualization.events);
            conf_view.renderFilterNodes();
        });
        setModeKeyBinding(filteringMode, "Regular Expression", function(){
            var sel = dialogRender();
            var conf_view = new ConfigurationView(null,sel,configuration,visualization.events);
            conf_view.renderRegexFilter();
        });
        setModeKeyBinding(filteringMode, "Probability", function(){
            var sel = dialogRender();
            var conf_view = new ConfigurationView(null,sel,configuration,visualization.events);
            conf_view.renderFilterProbabilty();
        });
    }


    function loadModeKeyBindings(mode) {
        currentKeyBindings = configuration.currentConfig.keyBindings.allwaysActive.concat(mode.keyBindings);
    }


    function keyMatch(keys, binding) {
        var keysl = keys.length - 1;
        var keysb = binding.length - 1;
        while (keysl >= 0 && keysb >= 0) {
            if (keys[keysl] != binding[keysb]) {
                return false;
            }
            --keysl;
            --keysb;
        }
        return keysb == -1;
    }

    function exitLastMode() {
        if (currentMode !== null && currentMode.hasOwnProperty('exitHandler')) {
            currentMode.exitHandler();
        }
    }

    function gotoStartingMode() {
        exitLastMode();
        currentKeyBindings = configuration.currentConfig.keyBindings.allwaysActive;
    }

    function gotoZoomMode() {
        exitLastMode();
        currentMode = getModeByName("Zoom");
        loadModeKeyBindings(currentMode);
        $('#zoomContainer').show();
    }

    function gotoFilteringMode() {
        exitLastMode();
        currentMode = getModeByName("Filtering");
        loadModeKeyBindings(currentMode);
    }

    setKeyBinding("Show Key bindings", function(){showKeybindingsPopup(configuration)});
    setKeyBinding("Move Left", zoomEvents.left);
    setKeyBinding("Move Up", zoomEvents.up);
    setKeyBinding("Move Right", zoomEvents.right);
    setKeyBinding("Move Down", zoomEvents.down);
    setKeyBinding("Move Down", zoomEvents.down);
    setKeyBinding("Undo", visualization.stateManager.undo);
    setKeyBinding("Redo", visualization.stateManager.redo);
    setKeyBinding("Goto Sunburst", function() {
        visualization.events.switchToViz(0, null)
    });
    setKeyBinding("Goto Vertical Partition", function() {
        visualization.events.switchToViz(1, null)
    });
    setKeyBinding("Goto Table", function() {
        visualization.events.switchToViz(2, null)
    });
    setKeyBinding("Goto Configurations", function() {
        visualization.events.switchToViz(3, null)
    });
    setKeyBinding("Goto starting mode", gotoStartingMode);
    setKeyBinding("Zoom Mode", gotoZoomMode);
    setKeyBinding("Filtering Mode", gotoFilteringMode);

    initZoomMode();
    initFilteringMode();


    document.onkeydown = function(e) {
        pressedKeys.push(e.keyCode);
        keyBinding = getKeyBindingByKeyCodes(pressedKeys);
        if (keyBinding !== null) {
            keyBinding.func();
            pressedKeys = [];
        }
    };
}