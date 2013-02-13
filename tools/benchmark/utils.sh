function min(){
  if [[ $1 < $2 ]]; then
	echo $1
  else
	echo $2
  fi
}

function max(){
  if [[ $1 > $2 ]]; then
	echo $1
  else
	echo $2
  fi
}
