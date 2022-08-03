for entry in "test"/*
do

  if [[ ${entry} =~ ^(.*)\.do$ ]]; then
    f="$(basename -- $entry)"

    ./dolang $entry > "test/result/"$f".result"
  fi

done