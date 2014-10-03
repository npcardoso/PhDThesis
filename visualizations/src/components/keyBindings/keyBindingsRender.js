function renderKeyCodesArray(keyCodes) {
    var renderStr = '';
    for (var i = 0; i < keyCodes.length; i++) {
        renderStr += KeyboardJS.key.name(keyCodes[i])[0] + ' ';
    };
    return renderStr;
}

function renderKeyBindingsDisplay(configuration, renderChange) {

    function keyBindingsRender(arrayKeyBindings, modeID) {
        var renderStr = '';
        for (var i = 0; i < arrayKeyBindings.length; i++) {
            renderStr += '<p><small>' + arrayKeyBindings[i].name + ': ';
            renderStr += renderKeyCodesArray(arrayKeyBindings[i].keyCodes);
            if (renderChange) {
                renderStr += ' <a id="mode_' + modeID + '_' + i + '" href="#" class="keyChange">Change</a>';
            }
            renderStr += '</small></p>';
        }
        return renderStr;
    }

    function modesRender(modes) {
        var str = '';
        for (var i = 0; i < modes.length; i++) {
            str += '<br /><small>' + modes[i].name + ' Mode</small>';
            str += keyBindingsRender(modes[i].keyBindings, i);
        }
        return str;
    }


    return keyBindingsRender(configuration.currentConfig.keyBindings.allwaysActive, -1) +
        modesRender(configuration.currentConfig.keyBindings.modes);
}


function showKeybindingsPopup(configuration) {
    $("#dialog").remove();
    $('body').append('<div id="dialog" title="Key Bindings"></div>');
    $("#dialog").dialog({
        autoOpen: true,
        modal: false,
        height: "200",
        show: {
            effect: "blind",
            duration: 1000
        },
        hide: {
            effect: "explode",
            duration: 1000
        }
    });
    var conf_view = new ConfigurationView(null, "#dialog", configuration, null);
    conf_view.renderKeyBindings();
    $('.keyBindingsp').remove();

}