function keyBindings(visualization,configuration){
	console.log(configuration.currentConfig);
	var zoomEvents = visualization.zoomEvents;

	var keyBindingsD = configuration.currentConfig.keyBindings;

	function getKeyBindingByName(name){
		for (var i = keyBindingsD.length - 1; i >= 0; i--) {
			if(keyBindingsD[i].name == name){
				return keyBindingsD[i];
			}
		};
		return null;
	}

	function getKeyBindingByKeyCodes(keyCodes){
		for (var i = keyBindingsD.length - 1; i >= 0; i--) {
			if(keyMatch(keyCodes,keyBindingsD[i].keyCodes)){
				return keyBindingsD[i];
			}
		};
		return null;
	}




	function setKeyBinding(name,func){
		getKeyBindingByName(name).func = func;
	}


	setKeyBinding("Zoom Reset",zoomEvents.zoomReset);
	setKeyBinding("Zoom In",zoomEvents.zoomIn);
	setKeyBinding("Move Left",zoomEvents.left);
	setKeyBinding("Move Up",zoomEvents.up);
	setKeyBinding("Zoom Out",zoomEvents.zoomOut);
	setKeyBinding("Move Right",zoomEvents.right);
	setKeyBinding("Move Down",zoomEvents.down);
	setKeyBinding("Move Down",zoomEvents.down);
	setKeyBinding("Undo",visualization.stateManager.recoverState);
	setKeyBinding("Goto Sunburst",function(){visualization.events.switchToViz(0,null)});
	setKeyBinding("Goto Vertical Partition",function(){visualization.events.switchToViz(1,null)});
	setKeyBinding("Goto Table",function(){visualization.events.switchToViz(2,null)});
	setKeyBinding("Goto Configurations",function(){visualization.events.switchToViz(3,null)});
	console.log(visualization);

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

function arrayEquals(array1,array2){
	if(array1.length != array2.length)
		return false;
	for (var i = array1.length - 1; i >= 0; i--) {
		if(array1[i] != array2[i]){
			return false;
		}
	};
	return true;
}