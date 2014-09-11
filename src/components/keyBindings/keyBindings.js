function keyBindings(visualization,configuration){
	console.log(configuration.currentConfig);
	var zoomEvents = visualization.zoomEvents;

	var currentKeyBindings = configuration.currentConfig.keyBindings.allwaysActive;


	function findByName(name,array){
		console.log(name);
		//console.log(currentKeyBindings);
		for (var i = array.length - 1; i >= 0; i--) {
			if(array[i].name == name){
				return array[i];
			}
		};
		return null;
	}

	function getKeyBindingByName(name){
		return findByName(name,currentKeyBindings);
	}

	function getModeByName(name){
		return findByName(name,configuration.currentConfig.keyBindings.modes);
	}


	function getKeyBindingByKeyCodes(keyCodes){
		for (var i = currentKeyBindings.length - 1; i >= 0; i--) {
			if(keyMatch(keyCodes,currentKeyBindings[i].keyCodes)){
				return currentKeyBindings[i];
			}
		};
		return null;
	}

	function setKeyBinding(name,func){
		getKeyBindingByName(name).func = func;
	}

	function setModeKeyBinding(mode,keyBindingName,func){
		var keyBinding = findByName(keyBindingName,mode.keyBindings);
		keyBinding.func = func;
	}

	function initZoomMode(){
		zoomMode = getModeByName("Zoom");
		setModeKeyBinding(zoomMode,"Zoom In",zoomEvents.zoomIn);
		setModeKeyBinding(zoomMode,"Zoom Out",zoomEvents.zoomOut);
		setModeKeyBinding(zoomMode,"Zoom Reset",zoomEvents.zoomReset);
	}


	function keyMatch(keys,binding){
		var keysl = keys.length-1;
		var keysb = binding.length-1;
		while(keysl >= 0 && keysb >= 0){
			if(keys[keysl] != binding[keysb]){
				return false;
			}
			--keysl;
			--keysb;
		}
		return keysb == -1;
	}

	function gotoStartingMode(){
		currentKeyBindings = configuration.currentConfig.keyBindings.allwaysActive;
	}

	function gotoZoomMode(){
		console.log(currentKeyBindings);
		zoomMode = getModeByName("Zoom");
		currentKeyBindings = configuration.currentConfig.keyBindings.allwaysActive.concat(zoomMode.keyBindings);
		console.log(currentKeyBindings);
	}

	setKeyBinding("Move Left",zoomEvents.left);
	setKeyBinding("Move Up",zoomEvents.up);
	setKeyBinding("Move Right",zoomEvents.right);
	setKeyBinding("Move Down",zoomEvents.down);
	setKeyBinding("Move Down",zoomEvents.down);
	setKeyBinding("Undo",visualization.stateManager.recoverState);
	setKeyBinding("Goto Sunburst",function(){visualization.events.switchToViz(0,null)});
	setKeyBinding("Goto Vertical Partition",function(){visualization.events.switchToViz(1,null)});
	setKeyBinding("Goto Table",function(){visualization.events.switchToViz(2,null)});
	setKeyBinding("Goto Configurations",function(){visualization.events.switchToViz(3,null)});
	setKeyBinding("Goto starting mode",gotoStartingMode);
	setKeyBinding("Zoom Mode",gotoZoomMode);

 	initZoomMode();


	var pressedKeys = [];
	//var timeOut;
	document.onkeydown = function(e) {
		//clearTimeout(timeOut);
		pressedKeys.push(e.keyCode);
		keyBinding = getKeyBindingByKeyCodes(pressedKeys);
		if(keyBinding != null){
			keyBinding.func();
			pressedKeys = [];
		}


	}

}


