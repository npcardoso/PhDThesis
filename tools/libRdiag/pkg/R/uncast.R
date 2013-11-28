library('gdata')
library('reshape2')
library('gdata')

read.data = function(path, generator.rename=NULL, ranker.rename=NULL) {
  data = read.csv(path)

  if(length(generator.rename))
    data$generator = factor(data$generator, labels=generator.rename)
  if(length(ranker.rename)) {
    data$ranker = factor(data$ranker, labels=ranker.rename)
  }

  casted_data = cast(data, randomizer + iteration + generator + ranker ~metric_name)
  colnames(data) = trim(colnames(data))
  colnames(casted_data) = trim(colnames(casted_data))


  return (list(casted=casted_data, melted=data))
}


get.data = function(data, rankers=NULL, generators=NULL, randomizers=NULL) {
  elements = 1:nrow(data)

  if(length(rankers))
    elements = intersect(elements, which(data$ranker %in% rankers))

  if(length(generators))
    elements = intersect(elements, which(data$generator %in% generators))

  if(length(randomizers))
    elements = intersect(elements, which(data$randomizer %in% randomizers))


  return (data [elements, ])

}


boom.selectah = function(data, randomizers=NULL){
  if(length(randomizers)) {
    elements = which(data$randomizer %in% randomizers)
    data = data[elements,]
  }


  data = data[!is.nan(data$value),]

  casted_data = cast(data, generator + ranker ~metric_name, mean, rm.na=T)
  return (casted_data)
}

boom.selectah.plot = function(data) {
  data = data [which(!is.na(data$value)), ]

  p = qplot(value, data=data, geom="density", fill=ranker, alpha=I(.5))
  p = p + facet_wrap(~metric_name, scales="free")

  return(p)
}
