package io.crowbar.diagnostic.handlers.spectrum;

import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.diagnostic.spectrum.Tree;

import com.wordnik.swagger.annotations.ApiModel;
import com.wordnik.swagger.annotations.ApiModelProperty;
import java.util.List;
import java.util.AbstractList;

@ApiModel(value = "Tree",
          description = "Tree resource representation")
public class TreeModel {
    private final Tree original;
    public TreeModel (Tree original) {
        this.original = original;
    }

    @ApiModelProperty(value = "Number of nodes on the tree", required = false)
    public int getNodeCount () {
        return original.size();
    }

    @ApiModelProperty(value = "Tree's nodes", required = true)
    public final List<NodeModel> getNodes () {
        return new AbstractList<NodeModel> () {
                   @Override
                   public int size () {
                       return original.size();
                   }

                   @Override
                   public NodeModel get (int i) {
                       Node n = original.getNodes().get(i);


                       if (n == null)
                           return null;

                       return new NodeModel(n);
                   }
        };
    }
}