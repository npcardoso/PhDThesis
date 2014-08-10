var xas;
function Gradiant(normal){
	var normalGradiante = this.gradianteInit(normal);

    this.normal = function(node){
        if(node.properties.p >= 0){
            return normalGradiante[Math.floor(node.properties.p*100)];
        }
        return "grey";
    }
}



function convertGradType(gradiante){
        xas = ret;
    var ret = [];
    var len = gradiante.length;
    for (var i = 0; i < len; i++) {
        ret[i] = rgba(gradiante[i].r,gradiante[i].g,gradiante[i].b,1);
    };

    return ret;
}

Gradiant.prototype.gradianteInit = function(gradiante){
	var grandianteComputed = new Array(101);
	var gradianteObj = this.gradienteCalc([0, 0.5, 1],convertGradType(gradiante));
	var colors;
	for (var i = grandianteComputed.length - 1; i >= 0; i--) {
		colors = gradianteObj.get(i/100);
		grandianteComputed[i] = "rgba(" + colors[0] + ", " + colors[1] + ", " + colors[2] + ", " + colors[3] + ")";
	}
	return grandianteComputed;
}

Gradiant.prototype.gradienteCalc = function(stops,colors) {
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