function StateManager(visualization){
	var self = this;
	var stack = [];


	this.initRender = function(elementSel){
		$(elementSel).append('<div class="iButtons"><img src="components/stateManager/backButton.png" /></div>');
		$('.iButtons').click(self.recoverState);
		self.setButtonVis();
	}

	this.setButtonVis = function(){
		if(self.undoPossible()){
			$('.iButtons').css('display','block');
		}
		else {
			$('.iButtons').css('display','none');
		}

	}

	this.getLast = function(){
		if(self.undoPossible()){
			return stack.slice(-1)[0];
		}
		return null;
	}

	this.undoPossible = function(){
		return stack.length > 0;
	}

	this.saveState = function (){
		stack.push([visualization.clicked,visualization.zoomEvents.getZoom()]);
		self.setButtonVis();
	}

	this.recoverState = function(){
		if(!self.undoPossible()){
			return;
		}
		lastState = self.getLast();
		if(visualization.gotoNode(lastState[0],visualization.configuration.currentConfig.animationTransitionTime) != false){
			visualization.zoomEvents.setZoom(lastState[1]);
			stack.pop();
		 	self.setButtonVis();
		}
		return;
	}
}