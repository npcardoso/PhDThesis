function Table(data, elementSel, configuration, events) {
    var self = this;
    
    

    this.getTableRows = function(){
        var ret = [];
        for (len = data.length, i = 0; i < len; i++) {
            var node = data[i];
            if(isLastNode(node))
            {
                ret.push([node.parent.parent.name,node.parent.name,node.name,node.properties.p,i]);
            }
        }
        return ret;
    }


    var tableData = this.getTableRows();
    this.render = function(){

        $(elementSel).html('<table cellpadding="0" cellspacing="0" border="0" class="display" id="tableData"></table>');

        var table = $('#tableData').DataTable({
            "retrieve" : true,
            "data" : tableData,
            "columns" : [{
                "title" : "Class"
            }, {
                "title" : "Function"
            }, {
                "title" : "Line",
                "class" : "center"
            }, {
                "title" : "Error Score",
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
            return '<table cellpadding="5" cellspacing="0" border="0" style="padding-left:50px;">'+
        '<tr>'+
            '<td>Tests Passed:</td>'+
            '<td>45</td>'+
        '</tr>'+
        '<tr>'+
            '<td>Failed testes:</td>'+
            '<td>3</td>'+
        '</tr>'+
        '<tr>'+
            '<td>Extra info:</td>'+
            '<td>Code coverege, statistics or other info</td>'+
        '</tr>'+
    '</table>'+'<div class="ui-widget-header ui-corner-all toolbar"><span id="buttons'+data[4]+'">'
    +'<input type="radio" id="viz0" name="viz0"><label for="viz0">Go to Sunburst</label>'
    +'<input type="radio" id="viz1" name="viz1"><label for="viz1">Go to Vertical Partition</label></span></div>';

        }

        table.on('click', '.tableCircle', function () {
            console.log(table);
            alert('ok');
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

            $( '#buttons'+row.data()[4]).buttonset();
        }
    } );

    }

}