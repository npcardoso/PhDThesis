fuzzinel = function(A, e, D) {
  stopifnot(nrow(A) == length(e))

  return(.Call('fuzzinel',
               as.matrix(A),
               as.vector(e),
               D))
}
