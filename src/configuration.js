function Configuration() {
  this.CONFIG_STORE = 'configv24';
  this.DEFAULT_CONFIG = {
    defaultView: 0,
    animationTransitionTime: 700,
    zoomAnimationTime: 500,
    filterMostRelevamtNodes: 400,
    filterMinProbability: 0,
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
   scriptsLoad: [],
   scriptsLoadAutoComplete: ["data_ex.js", "data_ex2.js"],
   regexFilter: "",
   defaultTableEntries: 100,
   keyBindings : [{
    name: "Zoom Reset",
    keyCodes: [90,82]
  },
  {
    name: "Zoom In",
    keyCodes: [90,73]
  },
  {
    name: "Zoom Out",
    keyCodes: [90,79]
  },
  {
    name: "Move Left",
    keyCodes: [37]
  },
  {
    name: "Move Up",
    keyCodes: [38]
  },
  {
    name: "Move Right",
    keyCodes: [39]
  },
  {
    name: "Move Down",
    keyCodes: [40]
  },
  {
    name: "Undo",
    keyCodes: [66]
  },
  {
    name: "Goto Sunburst",
    keyCodes: [49]
  },
  {
    name: "Goto Vertical Partition",
    keyCodes: [50]
  },
  {
    name: "Goto Table",
    keyCodes: [51]
  },
  {
    name: "Goto Configurations",
    keyCodes: [52]
  }
  ]
};

var self = this;

this.saveConfig = function() {
  localStorage.setItem(this.CONFIG_STORE, JSON.stringify(self.currentConfig));
}

this.saveGradiante = function() {
  self.saveConfig();
  self.gradiante = new Gradiant(self.currentConfig.normalGradiante);
}

this.resetConfig = function(){
  self.currentConfig = jQuery.extend(this.DEFAULT_CONFIG,  {
    lastViewed: this.DEFAULT_CONFIG.defaultView
  });
  self.saveGradiante();
}

var localStorageConf = localStorage.getItem(this.CONFIG_STORE);
if (localStorageConf == null || !(localStorageConf = JSON.parse(localStorageConf))) {
  self.resetConfig();
}
else {
  this.currentConfig = localStorageConf;
}
    //[rgba(0, 255, 0, 1), rgba(255, 255, 0, 1), rgba(255, 0, 0, 1)]
    this.gradiante = new Gradiant(this.currentConfig.normalGradiante);

  }