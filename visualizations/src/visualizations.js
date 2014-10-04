function Visualizations(configuration, dataManager) {
    var self = this;
    var nodeToRender = null;
    var currentVisualizationN, currentVisualization;

    var events = {
        click: sendClickEvent,
        switchToViz: function(visN, node) {
            nodeToRender = node;
            $('#tabs').tabs({
                active: visN
            });
        },
        filtersUpdate: function(){
            dataManager.updatefilter();
            if(currentVisualizationN < 3){
            currentVisualization.data = dataManager.getData().tree[0];
            currentVisualization.render();
            }
        }
    };



    this.createVisualization = function(visN) {
        if (dataManager.getData() == undefined) {
            return new visualizations[visN].obj(null, '#' + getVizID(visN), configuration, events);
        }
        return new visualizations[visN].obj(dataManager.getData().tree, '#' + getVizID(visN), configuration, events);

    }


    this.prepareVisualization = function(visN) {
        configuration.currentConfig.lastViewed = visN;
        configuration.saveConfig();
        currentVisualizationN = visN;
        currentVisualization = self.createVisualization(visN);
    }

    this.setVisualization = function(visN) {
        if (dataManager.getData() != undefined || visN == 3) {
            self.prepareVisualization(visN);
            currentVisualization.render();
            if (nodeToRender != null) {
                currentVisualization.dblclick(nodeToRender);
                nodeToRender = null;
            }
        }
    }

    this.getInitVisN = function() {
        if (configuration.currentConfig.defaultView >= 0) {
            return configuration.currentConfig.defaultView;
        }
        return configuration.currentConfig.lastViewed;
    }

    this.resize = function() {
        if (currentVisualization.hasOwnProperty('resize')) {
            currentVisualization.resize();
        }
    }

    this.init = function() {
        self.setVisualization(self.getInitVisN());
    }



}