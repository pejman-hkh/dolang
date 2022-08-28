for entry in "testjs"/*
do

  if [[ ${entry} =~ ^(.*)\.js$ ]]; then
    f="$(basename -- $entry)"

    echo $entry
    ./dolang $entry > "testjs/result/"$f".result"
  fi

done