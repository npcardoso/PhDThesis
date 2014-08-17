function Visualizations(configuration){
	var self = this;


	var events = {
		click: visClickEv
	};
	var currentVisualizationN,currentVisualization;


	this.createVisualization = function(visN){
		return new visualizations[visN].obj(data_ex,'#'+getVizID(visN),configuration,events);
	}

	this.setVisualization = function(visN){
		configuration.currentConfig.lastViewed = visN;
		configuration.saveConfig();
		currentVisualizationN = visN;
		currentVisualization = self.createVisualization(visN);
		currentVisualization.render();
	}

	this.getInitVisN = function(){
		if(configuration.currentConfig.defaultView >= 0){
		  return configuration.currentConfig.defaultView;
		}
		return configuration.currentConfig.lastViewed;
	}

	this.resize = function(){
		if(currentVisualization.hasOwnProperty('resize')){
			currentVisualization.resize();
		}
	}

	this.init = function(){
		self.setVisualization(self.getInitVisN());
	}



}