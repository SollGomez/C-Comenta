script_dir=$(dirname "$0")

directorio=$(realpath "$0")
directorioEjecutable=$(dirname "$directorio")
directorioModulo=$(dirname "$directorioEjecutable")
directorioTp=$(dirname "$directorioModulo")

test="$1"

directorioConfig="$directorioTp/configs/$test"

directorioBin="$directorioModulo/bin/"


cd "$directorioBin"

./memoria "$directorioConfig/memoria.config"