for entry in "ext"/*
do
  f="$(basename -- $entry)"

  cd $entry
  make -B
  cd ../../
done
