function initializeVisualization(jsonData){
	$('body').html('');
	window.data_ex = jsonData;
	init();
}


function sendClickEvent(node){
	console.log(JSON.stringify({"type":"click","nodeId":node.id}));
	triggerEvent(JSON.stringify({"type":"click","nodeId":node.id}));
}

