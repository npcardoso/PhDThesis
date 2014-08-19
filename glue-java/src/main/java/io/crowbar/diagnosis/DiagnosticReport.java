package io.crowbar.diagnosis;

import java.lang.reflect.Type;
import java.util.List;
import java.util.ArrayList;

import flexjson.JSON;
import flexjson.ObjectFactory;
import flexjson.ObjectBinder;
import flexjson.JsonNumber;
import flexjson.JSONException;
import java.util.Map;


public final class DiagnosticReport {
    public static class JSONObjectFactory implements ObjectFactory {
        private final static String GEN_RESULTS="gen_results";
        private final static String RANK_RESULTS="rank_results";
        private final static String CONNECTIONS="connections";


        public Object instantiate(ObjectBinder context, Object value, Type targetType, Class targetClass) {
            DiagnosticReport dr = new DiagnosticReport();
            if(!(value instanceof Map))
                throw context.cannotConvertValueToTargetType(value, Map.class);

            Map valueMap = (Map) value;
            for (String k : new String[] {GEN_RESULTS,
                                          RANK_RESULTS,
                                          CONNECTIONS}) {
                if(!valueMap.containsKey(k))
                    throw new JSONException("Missing key: " + k);
            }

            for(Object o : getList(context, valueMap.get(GEN_RESULTS))) {
                dr.generatorResults.add(parseGeneratorResult(context, o));
            }

            for(Object o : getList(context, valueMap.get(RANK_RESULTS))) {
                dr.rankerResults.add(parseRankerResult(context, o));
            }

            for(Object o : getList(context, valueMap.get(CONNECTIONS))) {
                dr.connections.add((Connection)new ObjectBinder().bind(o, Connection.class));
            }

            return dr;
        }

        private List getList(ObjectBinder context, Object o) {
            if(!(o instanceof List))
                throw context.cannotConvertValueToTargetType(o, List.class);
            return (List) o;
        }

        private List<Candidate> parseGeneratorResult(ObjectBinder context, Object o) {
            if(o == null)
                return null;

            List list = getList(context, o);
            List ret = new ArrayList<Candidate>();

            for(Object c : list) {
                ret.add(new Candidate.JSONObjectFactory().instantiate(context, c, null, null));
            }

            return ret;
        }

        private List<Double> parseRankerResult(ObjectBinder context, Object o) {
            if(o == null)
                return null;

            List list = getList(context, o);
            List ret = new ArrayList<Double>();

            for(Object n : list) {
                if(!(n instanceof JsonNumber))
                    ret.add(Double.NaN);
                else
                    ret.add(((JsonNumber) n).doubleValue());
            }

            return ret;
        }
    }


    @JSON
    private List<List<Candidate>> getGeneratorResults () {
        return generatorResults;
    }

    @JSON
    private List<List<Double>> getRankerResults () {
        return rankerResults;
    }

    @JSON
    private List<Connection> getConnections () {
        return connections;
    }

    private List<List<Candidate> > generatorResults = new ArrayList<List<Candidate> > ();
    private List<List<Double> > rankerResults = new ArrayList<List<Double> > ();
    private List<Connection> connections = new ArrayList<Connection> ();
}
