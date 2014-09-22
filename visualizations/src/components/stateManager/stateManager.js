function StateManager(visualization) {
    var self = this;
    var stack = [];
    var currentNumber = 0;
    var maxRedo = 0;

    this.initRender = function(elementSel) {
        $(elementSel).append('<div class="iButtons"><img src="components/stateManager/backButton.png" /></div>');
        $('.iButtons').click(self.recoverState);
        self.setButtonVis();
    }

    this.setButtonVis = function() {
        if (self.undoPossible()) {
            $('.iButtons').css('display', 'block');
        } else {
            $('.iButtons').css('display', 'none');
        }

    }

    this.getLast = function() {
        if (self.undoPossible()) {
            return stack.slice(-1)[0];
        }
        return null;
    }

    this.undoPossible = function() {
        return currentNumber > 0;
    }

    this.redoPossible = function(){
        return maxRedo > 0;
    }

    this.saveState = function() {
        stack[currentNumber] = [visualization.clicked, visualization.zoomEvents.getZoom()];
        currentNumber++;
        maxRedo = 0;
        self.setButtonVis();
    }

    this.gotoState  = function(state){
        if (visualization.gotoNode(state[0], visualization.configuration.currentConfig.animationTransitionTime) != false) {
            visualization.zoomEvents.setZoom(state[1]);
            self.setButtonVis();
            return true;
        }
        return false;
    }

    this.recoverState = function() {
        if (!self.undoPossible()) {
            return;
        }
        if (gotoState(stack[currentNumber-1])) {
            currentNumber--;
            maxRedo++;
        }
    }

    this.redoState = function (){
        if (gotoState(stack[currentNumber+1])) {
            currentNumber++;
            maxRedo--;
        }
    }
}