function NodeInfoDisplay(elementSel, clickFunction, configuration) {
    var self = this;

    this.init = function() {
        $('#breadcrumbs').remove();
        $('#endlabel').remove();
        $(elementSel).append('<div id="breadcrumbs"></div>');
        $(elementSel).append('<div id="endlabel"></div>');
    };

    this.nodeInfo = function(node) {
        self.updateScore(node);
        self.breadcrumbsRender(getAncestors(node));
    };


    this.breadcrumbsRender = function(nodesArray) {
        function idNodeI(i) {
            return 'node-' + i;
        }

        $('#breadcrumbs').html('');
        $('.breadcrumbsStyle').remove();
        $('.breadcrumbs').remove();
        var renderElement = $('<ol class="breadcrumbs"></ol>');
        $('#breadcrumbs').html(renderElement);

        var lengthN = nodesArray.length;
        for (var i = 0; i < lengthN; i++) {
            var elem = $('<li><a id="' + idNodeI(i) + '"><span>' + (false ? '' : nodesArray[i].n) + '</span></a></li>');
            if (isLastNode(nodesArray[i])) {
                elem.addClass("leaf-node");
            }
            renderElement.append(elem);
            var x = $('body').append('<style class="breadcrumbsStyle">#' + idNodeI(i) + ', #' + idNodeI(i) + ':before, #' + idNodeI(i) + ':after{background-color: ' + configuration.gradiante.normal(nodesArray[i]) + ';}</style>');
            $('a', renderElement).click(function() {
                var id = $(this).attr('id').replace("node-", "");
                clickFunction(nodesArray[id]);
            })
        };

        var i = 0;
        console.log(renderElement.height());
        while (renderElement.height() > 30) {
            console.log(renderElement.height());
            $('#' + idNodeI(i)).remove();
            ++i;
        }
    };

    this.simpleBreadCrumbsRender = function(nodesArray) {

    };


    this.updateScore = function(node) {
        var color = configuration.gradiante.normal(node);
        // Now move and update the percentage at the end.
        var percentage, displayText;
        if (node.score >= 0) {
            percentage = d3.round(node.score * 100, 2);
            displayText = percentage + '%';
        } else {
            percentage = 0;
            displayText = 'Uncalculated';
        }
        $("#endlabel").html('<div class="pace pace-active" style="border-color: ' + color + ';"><div class="pace-progress" data-progress="' + percentage + '" data-progress-text="' + displayText + '" style="width: ' + percentage + '%;   background: ' + color + '; color: ' + color + ';"><div class="pace-progress-inner"></div></div><div class="pace-activity"></div></div>');
    };


    this.updataBreadcumbTimed = function(node) {
        if (self.updateBreadcumbTimeOut !== undefined) {
            clearTimeout(self.updateBreadcumbTimeOut);
        }
        self.updateBreadcumbTimeOut = setTimeout(function() {
            self.nodeInfo(node);
        }, 250);
    };


    this.setClicked = function(node) {
        self.clickedNode = node;
        self.nodeInfo(node);
    };

    this.setPath = function(path) {
        self.path = path;
    };

    this.mouseover = function(node) {
        self.updataBreadcumbTimed(node);
        var sequenceArray = getAncestors(node);
        self.path.style("opacity", 0.3);

        self.path.filter(function(node) {
                return (sequenceArray.indexOf(node) >= 0);
            })
            .style("opacity", 1);
    };


    this.mouseleave = function(node) {
        self.updataBreadcumbTimed(self.clickedNode);
        //path.on("mouseover", null);
        self.path.style("opacity", 1);
        return;
        self.path.transition()
            .duration(1000)
            .style("opacity", 1)
            .each("end", function() {
                d3.select(this).on("mouseover", self.mouseover);
            });
    };

    this.init();

}