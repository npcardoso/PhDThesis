function Configuration(){
	this.DEFAULT_CONFIG = {
		defaultView: 4,
		animationTransitionTime: 750
	};

	var self = this;
	var localStorageConf = localStorage.getItem('config');
	if(localStorageConf == null || !( localStorageConf = JSON.parse(localStorageConf))){
		this.currentConfig = this.DEFAULT_CONFIG;
	}
	else
	{
		this.currentConfig = localStorageConf;
	}

	this.gradiante = new Gradiant();

	this.saveConfig = function(){
		localStorage.setItem('config', JSON.stringify(self.currentConfig));
	}
}