function Gradiant(normal){
	var normalGradiante = this.gradianteInit(normal);

    this.normal = function(node){
        if(node.score >= 0){
            return normalGradiante[Math.floor(node.score*100)];
        }
        return "grey";
    }
}


function colorToRga(color){
    colorObj = tinycolor(color).toRgb();
    return [colorObj.r,colorObj.g,colorObj.b,colorObj.a];
}

Gradiant.prototype.gradianteInit = function(gradiente){
	var stops = [];
    var colors = [];
    for (i = 0, len = gradiente.length; i < len; i++) {
        stops.push(gradiente[i].position/100);
        colors.push(colorToRga(gradiente[i].color));
    };
	var gradianteObj = this.gradienteCalc(stops,colors);
    var grandianteComputed = [];
	var rgba;
	for (var i = 0; i <= 100; i++) {
		rgba = gradianteObj.get(i/100);
		grandianteComputed.push("rgba(" + rgba[0] + ", " + rgba[1] + ", " + rgba[2] + ", " + rgba[3] + ")");
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


function inputToRGB(color) {

    var rgb = { r: 0, g: 0, b: 0 };
    var a = 1;
    var ok = false;
    var format = false;

    if (typeof color == "string") {
        color = stringInputToObject(color);
    }

    if (typeof color == "object") {
        if (color.hasOwnProperty("r") && color.hasOwnProperty("g") && color.hasOwnProperty("b")) {
            rgb = rgbToRgb(color.r, color.g, color.b);
            ok = true;
            format = String(color.r).substr(-1) === "%" ? "prgb" : "rgb";
        }
        else if (color.hasOwnProperty("h") && color.hasOwnProperty("s") && color.hasOwnProperty("v")) {
            color.s = convertToPercentage(color.s);
            color.v = convertToPercentage(color.v);
            rgb = hsvToRgb(color.h, color.s, color.v);
            ok = true;
            format = "hsv";
        }
        else if (color.hasOwnProperty("h") && color.hasOwnProperty("s") && color.hasOwnProperty("l")) {
            color.s = convertToPercentage(color.s);
            color.l = convertToPercentage(color.l);
            rgb = hslToRgb(color.h, color.s, color.l);
            ok = true;
            format = "hsl";
        }

        if (color.hasOwnProperty("a")) {
            a = color.a;
        }
    }

    a = parseFloat(a);

            // Handle invalid alpha characters by setting to 1
            if (isNaN(a) || a < 0 || a > 1) {
                a = 1;
            }

            return {
                ok: ok,
                format: color.format || format,
                r: mathMin(255, mathMax(rgb.r, 0)),
                g: mathMin(255, mathMax(rgb.g, 0)),
                b: mathMin(255, mathMax(rgb.b, 0)),
                a: a
            };
        }