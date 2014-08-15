function Configuration() {
    this.DEFAULT_CONFIG = {
        defaultView: 0,
        animationTransitionTime: 700,
        filterMostRelevamtNodes: 700,
        normalGradiante: [
        {
           color: "green",
           position: 1
       },
        {
           color: "yellow",
           position: 50
       },
       {
           color: "rgba(255,0,0,1)",
           position: 99
       }
       ],
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