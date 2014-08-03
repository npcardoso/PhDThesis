//Program entry point
function init(){
	//Calculate rendering dimensions (dependent on css) it will be dynamic on future
	var width= $(window).width()-75;
	var height = $(window).height()-115;

	renderButtonsHtml(width,height);

	//prepare data
	dataInlining(data_ex);
	probabilityCalculator(data_ex[0]);


	//Render the default visualization
	visualizations[0].init("#tabs-0",width,height);
}

//Render html of the buttons
function renderButtonsHtml(width,height){
	$("body").append('<div id="tabs"><ul></ul></div>');
	$.each(visualizations, function(index, visualization) {
		$("#tabs ul").append('<li><a href="#tabs-'+index+'">'+visualization.displayName+'</a></li>');
		$("#tabs").append('<div id="tabs-'+index+'"></div>');
	});

	$( "#tabs" ).tabs({
		activate: function( event, ui ) {
			var visualizationIndex = ui.newTab.children("a").attr("href").replace("#tabs-","");
			visualizations[visualizationIndex].init("#tabs-"+visualizationIndex,width,height);
		}
	});
}

//Function called when a node is clicked on the visualization Now is just shows an alert
function visClickEv(node) {
	var anscestors = getAncestors(node);
	var str = "";
	var len = anscestors.length;
	for (var i = 0; i < len; i++) {
		str += anscestors[i].name + " -> ";
	};
	str += node.name;
	alert(str);
}

//Helper function to calculate the color of a node
function calculateColor(d) {
	if(!calculateColor.hasOwnProperty('gradiant')){
		calculateColor.gradiant = new Gradiant();
	}
	return calculateColor.gradiant.normal(d);
}
