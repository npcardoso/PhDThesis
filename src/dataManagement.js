function dataInlining (data) {
	for (var i = data.length - 1; i >= 0; i--) {
		if(data[i].hasOwnProperty('children') )
		{
			for (var j = data[i].children.length - 1; j >= 0; j--) {
				data[i].children[j] = data[data[i].children[j]];
			};
		}
		if(data[i].parent_id >= 0){
			data[i].parent = data[data[i].parent_id];
		}
	};	
}

function getAncestors(node){
	if(node.hasOwnProperty('parent')){
		var parentAncestors = getAncestors(node.parent);
		parentAncestors.push(node);
		return parentAncestors;
	}
	return new Array(node);
}

function removeArray(arr, item) {
	for(var i = arr.length; i--;) {
		if(arr[i].id == item) {
			arr.splice(i, 1);
		}
	}
}


function isLastNode(node){
	return !node.hasOwnProperty('children') || node.children == null || node.children.length == 0;
}

function getLastLevelNodes(data){
	return data.filter(isLastNode);
} 

function filterTrue(nodes){
	return nodes.filter(function(node){
		return  node != undefined && node != null && node.hasOwnProperty('state') && node.state == true;
	});
}

function treeFilter(root){
	if(root == null || root == undefined){
		return;
	}
	if(!isLastNode(root)){
	root.children = filterTrue(root.children);
	for (var i = root.children.length - 1; i >= 0; i--) {
		treeFilter(root.children[i]);
	}
	}
}

function filterData(data,N){
	var array = getLastLevelNodes(data);
	array.sort(function(a,b){
		return b.properties.p - a.properties.p;
	});
	var end = Math.min(N,array.length);
	console.log(end);
	for (var i = 0; i < end; i++) {
		var nodes = getAncestors(array[i]);
		//console.log(nodes);
		for (var j = nodes.length - 1; j >= 0; j--) {
			nodes[j].state = true;
			//console.log(nodes[j]);
		};
	};
	treeFilter(data[0]);
	return filterTrue(data);
}


function filterToNMostRelevant(data,N){
	var tmp = data.slice();
	tmp.sort(function(a,b){
		return b.properties.p - a.properties.p;
	});
	var maxProb = tmp[Math.min(tmp.length-1,N)].properties.p;
	probabilityFilter(data,maxProb);
}

function probabilityFilter(data,pFilter){
	for (var i = data.length - 1; i >= 0; i--) {
		if(data[i].properties.p < pFilter)
		{
			var parent = data[data[i].parent_id];
			if(parent != null && parent.hasOwnProperty('children')){
				removeArray(parent.children,data[i].id);
			}
			data.splice(i, 1);
		}
	}
}

function probabilityCalculator(node) {
	if(node.hasOwnProperty('properties') && node.properties.hasOwnProperty('p'))
		return node.properties.p;

	var p = -1;
	if(node.hasOwnProperty('children')){
		for (var i = node.children.length - 1; i >= 0; i--) {
			p = Math.max(p,probabilityCalculator(node.children[i]));
		};
	}

	if(node.hasOwnProperty('properties')){
		node.properties.p = p;
	}
	else
	{
		node.properties = {
			p: p 
		};
	}
	
	return p;
}


function randomProbabilityInjector(data){
	for (var i = data.length - 1; i >= 0; i--) {
		if(!data[i].hasOwnProperty('children') )
		{
			data[i].properties = {
				p: Math.floor((Math.random() * 101))/100
			};
		}
	};	
}