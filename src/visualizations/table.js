function Table(data, elementSel, configuration, events) {
    var self = this;
    
    

    this.getTableRows = function(){
        var ret = [];
        for (len = data.length, i = 0; i < len; i++) {
            var node = data[i];
            if(isLastNode(node))
            {
                ret.push([node.parent.parent.n,node.parent.n,node.n,node.score,i]);
            }
        }
        return ret;
    }


    this.resize = function(){
        self.render();
    }

    var tableData = this.getTableRows();
    this.render = function(){

        $(elementSel).html('<table cellpadding="0" cellspacing="0" border="0" class="display" id="tableData"></table>');

        var table = $('#tableData').DataTable({
            "scrollY":        $(window).height()-205 + "px",
            "retrieve" : true,
            "scrollCollapse": true,
            "data" : tableData,
            "columns" : [{
                "title" : "Class"
            }, {
                "title" : "Function"
            }, {
                "title" : "Line",
                "class" : "center"
            }, {
                "title" : "E. S.",
                "class" : "center"
            }],
            "order" : [3, "desc"],
            "createdRow": function ( row, tdata, index ) {
                $('td', row).eq(0).prepend('<div class="tableCircle" style="background-color: '+configuration.gradiante.normal(data[tdata[4]])+';">+</div>');
            },
            "iDisplayLength": configuration.currentConfig.defaultTableEntries
        });

        table.on('click', 'tr', function() {
            if ($(this).hasClass('selected')) {
                $(this).removeClass('selected');
            } else {
                table.$('tr.selected').removeClass('selected');
                $(this).addClass('selected');
            }
            location.hash = "asa";
        });


        function format(data){
            return '<button class="vizb" id="vizb0">Go to Sunburst</button><button class="vizb" id="vizb1">Go to Vertical Partition</button>';

        }

        table.on('click', '.tableCircle', function () {
            console.log(table);
            var tr = $(this).closest('tr');
            var row = table.row( tr );

            if ( row.child.isShown() ) {
            // This row is already open - close it
            $(this).html('+');
            row.child.hide();
            tr.removeClass('shown');
        }
        else {
            $(this).html('-');
            // Open this row
            row.child( format(row.data()) ).show();
            tr.addClass('shown');

            $('.vizb').button().click(function(e) {
                var rawElement = this;
                var $element = $(this);
                var vizN = $element.attr('id').replace('vizb','');
                events.switchToViz(vizN,data[row.data()[4]]);
                return false;
                /* Do stuff with the element that fired the event */
            });
        }
    } );

    }

}