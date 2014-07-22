package io.crowbar.sandbox;


import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

public class Tree implements
java.io.Serializable,
Iterable<Tree> {
    public static class AlreadyRegisteredException extends Exception {
        AlreadyRegisteredException (String reason) {super(reason);}
    }

    public static class NotIdentifiableException extends Exception {
        NotIdentifiableException (String reason) {super(reason);}
    }


    private static final int ID_UNREGISTERED = -1;
    private static final int ID_ROOT = -2;


    /*!
     * \brief The node's name.
     * If the node represents:
     *  - A class: class name.
     *  - A method: method name
     *  - A line: line number
     */
    private String name;
    /*!
     * \brief The node's id.
     * This id should be -1 unless the node is registered.
     * The parent class is responsible for assigning this id upon registration.
     */
    private int id = ID_UNREGISTERED;
    /*!
     * \brief The node's parent.
     * This variable should be equal to null unless the node is registered.
     * If equal to null, the node is a root node.
     * The parent class is responsible for assigning this upon registration.
     */
    private Tree parent = null;
    /*!
     * \brief The tree's depth.
     * A leaf node has depth 0.
     */
    private int depth = 0;
    /*!
     * \brief The node's children, acessible by id.
     */
    private List<Tree> children_by_id = null; // ! A list of child nodes, where the index represents the child's id
    /*!
     * \brief The node's children, acessible by name.
     */
    private Map<String, Tree> children_by_name = null;
    /*!
     * \brief A map for additional node properties.
     */
    private Map<String, String> properties = null;

    Tree (String name) {
        this(name, false);
    }

    Tree (String name,
          boolean isRoot) {
        this.name = name;
        id = isRoot ? ID_ROOT : ID_UNREGISTERED;
    }

    public boolean isRegistered () {
        return id != ID_UNREGISTERED;
    }

    public boolean isRoot () {
        return id == ID_ROOT;
    }

    /*!
     * This method can only be called for registered nodes that are not root nodes.
     */
    public int getId () throws NotIdentifiableException {
        if (!isRegistered() || isRoot())
            throw new NotIdentifiableException("Trying to getId from node '" + getFullName() + "'. " +
                                               "isRegistered() == " + isRegistered() + ", " +
                                               "isRoot() == " + isRoot());

        return id;
    }

    public String getName () {
        return name;
    }

    /*!
     * \brief Concatenates the names of all nodes in the path to the root using ":" as separator.
     */
    public String getFullName () {
        return getFullName(":");
    }

    /*!
     * \brief Concatenates the names of all nodes in the path to the root using "separator" as separator.
     */
    public String getFullName (String separator) {
        if (isRoot())
            return name;

        return getParent().getFullName(separator) + separator + name;
    }

    public Tree getParent () {
        return parent;
    }

    public int getDepth () {
        return depth;
    }

    public Iterator<Tree> iterator () {
        if (children_by_id == null)
            return new ArrayList<Tree> ().iterator();

        return children_by_id.iterator();
    }

    public Tree getChild (int id) {
        if (children_by_id == null)
            return null;

        if (children_by_id.size() <= id)
            return null;

        if (id < 0)
            return null;

        return children_by_id.get(id);
    }

    public Tree getChild (String name) {
        return children_by_name.get(name);
    }

    /*!
     * \brief Sets the id and parent variables upon registration.
     *
     * This method should only be called by the parent node upon registration.
     * This method can only be called if the this node is not registered.
     */
    protected void register (int id,
                             Tree parent) throws AlreadyRegisteredException {
        if (isRegistered())
            throw new AlreadyRegisteredException("Trying to register node '" + getFullName() + "' which is " +
                                                 (isRoot() ? "a root node" : "already registered."));

        this.id = id;
        this.parent = parent;
    }

    /*!
     * \brief Registers an Tree as a child.
     *
     * This method can only be called if the both the parent and child
     * Trees are not registered.
     * @return registration id
     */
    public int register (Tree child) throws AlreadyRegisteredException {
        if (isRegistered() && !isRoot())
            throw new AlreadyRegisteredException("Registration node '" + getFullName() + "' is already registered.");

        if (children_by_id == null) // ! Only create list when needed
            children_by_id = new ArrayList<Tree> ();

        children_by_id.add(child);

        if (children_by_name == null) // ! Only create map when needed
            children_by_name = new HashMap<String, Tree> ();

        children_by_name.put(child.getName(), child);

        depth = Math.max(getDepth(), child.getDepth() + 1);

        int id = children_by_id.size() - 1;
        child.register(id, this);
        return id;
    }
}