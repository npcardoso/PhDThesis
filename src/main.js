//Program entry point
function init(){
	var configuration = new Configuration();
	$.each(configuration.currentConfig.scriptsLoad, function(index, script) {
		$.ajax({
			async: false,
			url: script,
			dataType: "script"
		});
	});
	//prepare data
	dataInlining(window.data_ex);
	if(configuration.currentConfig.filterMostRelevamtNodes > 0){
	window.data_ex = filterData(window.data_ex,configuration.currentConfig.filterMostRelevamtNodes);
	}
	probabilityCalculator(data_ex[0]);


	var visualization = new Visualizations(configuration);
	renderButtonsHtml(visualization.setVisualization,visualization.getInitVisN());

	visualization.init();

	$( window ).resize(visualization.resize);
}

function getVizID(vizNo){
	return 'tabs-'+vizNo;
}

//Render html of the buttons
function renderButtonsHtml(visActivationCallBack,defaultTab){
	$("body").append('<div id="tabs"><ul></ul></div>');
	$.each(visualizations, function(index, visualization) {
		$("#tabs ul").append('<li><a href="#'+getVizID(index)+'">'+visualization.displayName+'</a></li>');
		$("#tabs").append('<div id="'+getVizID(index)+'"></div>');
	});

	$( "#tabs" ).tabs({
		activate: function( event, ui ) {
			var visualizationIndex = ui.newTab.children("a").attr("href").replace("#tabs-","");
			visActivationCallBack(visualizationIndex);
		},
		active: defaultTab
	});

}


function getDimensions(){
	return {
		width: $(window).width()-75,
		height: $(window).height()-155
	}
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
	//alert(str);
}