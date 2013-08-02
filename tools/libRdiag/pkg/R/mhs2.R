mhs2 = function(A, e, max.runtime=-1, max.candidates=-1, max.candidate.size=-1) {
  stopifnot(nrow(A) == length(e))

  return(.Call('mhs2',
               as.matrix(A),
               as.vector(e),
               as.numeric(max.runtime),
               as.numeric(max.candidates),
               as.numeric(max.candidate.size)))
}

