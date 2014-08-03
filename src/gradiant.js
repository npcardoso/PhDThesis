function Gradiant(){
	var NOMAL_GRADIANT = {
		stops: [0, 0.5, 1],
		colors: [rgba(0, 255, 0, 1), rgba(255, 255, 0, 1), rgba(255, 0, 0, 1)]
	}


	var normalGradiant = this.gradiantInit(NOMAL_GRADIANT);

    this.normal = function(node){
        if(node.properties.p >= 0){
            return normalGradiant[Math.floor(node.properties.p*100)];
        }
        return "grey";
    }
}



Gradiant.prototype.gradiantInit = function(gradiant){
	var grandiantComputed = new Array(101);
	var gradiantObj = this.gradientCalc(gradiant.stops,gradiant.colors);
	var colors;
	for (var i = grandiantComputed.length - 1; i >= 0; i--) {
		colors = gradiantObj.get(i/100);
		grandiantComputed[i] = "rgba(" + colors[0] + ", " + colors[1] + ", " + colors[2] + ", " + colors[3] + ")";
	}
	return grandiantComputed;
}

Gradiant.prototype.gradientCalc = function(stops,colors) {
    obj = new Object();
    obj.stops = stops;
    obj.colors = colors;
    obj.get = function(n) {
        var n_stops = this.stops.length;
        var start = 0;
        var end = 0;
        var perc = 0;
        if (n <= this.stops[0]) end = start = this.colors[0];
        else if (n >= this.stops[n_stops - 1]) end = start = this.colors[n_stops - 1];
        else {
            for (var i = 1; i < n_stops; i++)
                if (n <= this.stops[i]) break;
            start = this.colors[i - 1];
            end = this.colors[i];
            perc = (n - this.stops[i - 1]) / (this.stops[i] - this.stops[i - 1]);
        }
        var ret = new Array();
        for (var i = 0; i < start.length; i++) {
            ret[i] = start[i] + (end[i] - start[i]) * perc;
            if (i != 3) ret[i] = Math.round(ret[i]);
        } /* console.log(n, ret, start, end);*/
        return ret;
    };
    return obj;
};

function rgba(r, g, b, a) {
    return new Array(r, g, b, a);
};