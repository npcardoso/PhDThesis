function Visualizations(){
	var self = this;

	var gradiante = new Gradiant();
	var events = {
		click: visClickEv
	};
	var currentVisualizationN,currentVisualization;


	this.createVisualization = function(visN){
		return new visualizations[visN].obj(data_ex[0],getVizID(visN),gradiante,events);
	}

	this.setVisualization = function(visN){
		currentVisualizationN = visN;
		currentVisualization = self.createVisualization(visN);
		currentVisualization.render();
	}

	this.resize = function(){
		if(currentVisualization.hasOwnProperty('resize')){
			currentVisualization.resize();
		}
	}

	this.init = function(){
		self.setVisualization(0);
	}



}