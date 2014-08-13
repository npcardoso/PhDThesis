function Configuration() {
    this.DEFAULT_CONFIG = {
        defaultView: 0,
        animationTransitionTime: 750,
        filterMostRelevamtNodes: 0,
        normalGradiante: [{
            r: 0,
            g: 255,
            b: 0
        }, {
            r: 255,
            g: 255,
            b: 0
        }, {
            r: 255,
            g: 0,
            b: 0
        }],
        scriptsLoad: ["data_ex.js"],
        scriptsLoadAutoComplete: ["data_ex.js", "data_ex2.js"]
    };

    var self = this;
    var localStorageConf = localStorage.getItem('config');
    if (localStorageConf == null || !(localStorageConf = JSON.parse(localStorageConf))) {
        this.currentConfig = this.DEFAULT_CONFIG;
    } else {
        this.currentConfig = localStorageConf;
    }


    //[rgba(0, 255, 0, 1), rgba(255, 255, 0, 1), rgba(255, 0, 0, 1)]
    this.gradiante = new Gradiant(this.currentConfig.normalGradiante);

    this.saveConfig = function() {
        localStorage.setItem('config', JSON.stringify(self.currentConfig));
    }

    this.saveGradiante = function() {
        self.saveConfig();
        self.gradiante = new Gradiant(self.currentConfig.normalGradiante);
    }

}