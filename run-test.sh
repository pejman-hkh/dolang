for entry in "test"/*
do

  if [[ ${entry} =~ ^(.*)\.do$ ]]; then
    f="$(basename -- $entry)"

    echo $entry
    ./do $entry > "test/result/"$f".result"
  fi

done