function initializeVisualizationREST(jsonData) {
    $('body').html('');

    tmp = JSON.parse(jsonData);

    window.data_ex = tmp.response
    init();
}

function initializeVisualization(jsonData) {
    $('body').html('');
    window.data_ex = JSON.parse(jsonData);
    init();
}


function sendClickEvent(node) {
    console.log(JSON.stringify({
        "type": "click",
        "nodeId": node.id
    }));
    if("triggerEvent" in window){
        triggerEvent(JSON.stringify({
            "type": "click",
            "nodeId": node.id
        }));
    }
}
