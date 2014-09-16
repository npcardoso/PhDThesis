function dataInlining(data) {
    var tree = data.tree;
    var scores = data.scores;
    var node;
    for (var i = tree.length - 1; i >= 0; i--) {
        node = tree[i];
        node.id = i;
        if (nodeHasFather(node)) {
            node.parent = tree[node.p];
        }
        node.score = scores[i];
        node.children = [];

    }
    for (var j = tree.length - 1; j >= 0; j--) {
        node = tree[j];
        if (nodeHasFather(node)) {
            node.parent.children.push(node);
        }
    }
}

function nodeHasFather(node) {
    return node.p >= 0;
}

function getName(node) {
    return node.n;
}

function getScore(node) {
    return node.score;
}

function getAncestors(node) {
    if (node.hasOwnProperty('parent')) {
        var parentAncestors = getAncestors(node.parent);
        parentAncestors.push(node);
        return parentAncestors;
    }
    return new Array(node);
}

function setState(nodesArray, state) {
    for (var i = nodesArray.length - 1; i >= 0; i--) {
        nodesArray[i].state = state;
    }
}

function getDescendents(node) {
    if (isLastNode(node))
        return [];
    var descendents = [];
    for (var i = node.children.length - 1; i >= 0; i--) {
        descendents = descendents.concat(getDescendents(node.children[i]));
    }
    return node.children.concat(descendents);
}

function getAncestorsAndDescends(node) {
    return getAncestors(node).concat(getDescendents(node));
}


function filterWithAncestorsAndDescents(nodesArray, filterFunction) {
    setState(nodesArray, false);
    for (var i = nodesArray.length - 1; i >= 0; i--) {
        if (filterFunction(nodesArray[i])) {
            setState(getAncestorsAndDescends(nodesArray[i]), true);
        }
    }
    treeFilter(nodesArray[0]);
    return filterTrue(nodesArray);
}


function regexFilter(nodesArray, regexStr) {
    var regex = new RegExp(regexStr);
    return filterWithAncestorsAndDescents(nodesArray, function(node) {
        return regex.test(getName(node));
    });
}

function removeArray(arr, item) {
    for (var i = arr.length; i--;) {
        if (arr[i].id == item) {
            arr.splice(i, 1);
        }
    }
}


function sortByProbability(NodesArray) {
    NodesArray.sort(function(a, b) {
        return b.score - a.score;
    });
}

function isLastNode(node) {
    return node === undefined || !node.hasOwnProperty('children') || node.children === null || node.children.length === 0;
}

function getLastLevelNodes(data) {
    return data.filter(isLastNode);
}

function filterTrue(nodes) {
    return nodes.filter(function(node) {
        return node !== undefined && node !== null && node.hasOwnProperty('state') && node.state === true;
    });
}

function treeFilter(root) {
    if (root === null || root === undefined) {
        return;
    }
    if (!isLastNode(root)) {
        root.children = filterTrue(root.children);
        for (var i = root.children.length - 1; i >= 0; i--) {
            treeFilter(root.children[i]);
        }
    }
}

function filterData(data, N) {
    setState(data, false);
    var array = getLastLevelNodes(data);
    sortByProbability(array);
    var end = Math.min(N, array.length);
    console.log(end);
    for (var i = 0; i < end; i++) {
        var nodes = getAncestors(array[i]);
        for (var j = nodes.length - 1; j >= 0; j--) {
            nodes[j].state = true;
        }
    }
    treeFilter(data[0]);
    return filterTrue(data);
}


function filterByScore(data, minScore) {
    minScore /= 100;
    setState(data, false);
    var array = getLastLevelNodes(data);
    var end = array.length;
    for (var i = 0; i < end; i++) {
        node = array[i];
        if (node.score >= minScore) {
            var nodes = getAncestors(node);
            //console.log(nodes);
            for (var j = nodes.length - 1; j >= 0; j--) {
                nodes[j].state = true;
            }
        }
    }
    treeFilter(data[0]);
    return filterTrue(data);
}


function probabilityCalculator(node) {
    if (node.score >= 0)
        return node.score;

    var p = 0;
    if (node.hasOwnProperty('children')) {
        for (var i = node.children.length - 1; i >= 0; i--) {
            p = Math.max(p, probabilityCalculator(node.children[i]));
        }
    }

    node.score = p;

    return p;
}


function randomProbabilityInjector(data) {
    for (var i = data.length - 1; i >= 0; i--) {
        if (!data[i].hasOwnProperty('children')) {
            data[i].properties = {
                p: Math.floor((Math.random() * 101)) / 100
            };
        }
    }
}


function DataManager(data, configuration) {
    var self = this;
    var original_data = jQuery.extend(true, {}, data);
    this.applyFilters = function() {
        if (data !== undefined) {
            dataInlining(data);
            probabilityCalculator(data.tree[0]);

            if (validRegex(configuration.currentConfig.regexFilter)) {
                data.tree = regexFilter(data.tree, configuration.currentConfig.regexFilter);
            }


            if (configuration.currentConfig.filterMinProbability > 0) {
                data.tree = filterByScore(data.tree, configuration.currentConfig.filterMinProbability);
            }

            if (configuration.currentConfig.filterMostRelevamtNodes > 0) {
                data.tree = filterData(data.tree, configuration.currentConfig.filterMostRelevamtNodes);
            }
        }
    };

    this.updatefilter = function() {
        data = jQuery.extend(true, {}, original_data);
        self.applyFilters();
    };

    this.getData = function() {
        return data;
    };
}