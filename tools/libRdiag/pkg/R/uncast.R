boom_selectah = function(file){
  data = read.csv(file)
  casted_data = cast(data, generator + ranker ~metric_name, mean)
  return (casted_data)
}
