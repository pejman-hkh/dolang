for entry in "testjs"/*
do

  if [[ ${entry} =~ ^(.*)\.js$ ]]; then
    f="$(basename -- $entry)"

    echo $entry
    ./doit $entry > "testjs/result/"$f".result"
  fi

done