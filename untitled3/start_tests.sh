#!/bin/bash

# Percorso del programma C già compilato
C_PROGRAM="./vm"  # Sostituisci con il percorso del tuo programma C

# Lista degli script bash da eseguire ciclicamente
script_list=(
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_01.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_02.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_03.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_04.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_05.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_06.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_07.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_08.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_09.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_10.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_11.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_12.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_13.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_14.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_15.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_16.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_17.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_18.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_19.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_20.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_21.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_22.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_23.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_24.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_25.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_26.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_27.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_28.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_29.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_30.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_31.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_32.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_33.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_34.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_35.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_36.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_37.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_38.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_39.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_40.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_41.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_42.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_43.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_44.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_45.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_46.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_47.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_48.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_49.txt"
    "/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_50.txt"
"/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_51.txt"
"/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_52.txt"
"/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_53.txt"
"/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_54.txt"
"/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_55.txt"
"/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_56.txt"
"/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_57.txt"
"/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_58.txt"
"/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_59.txt"
"/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_60.txt"
"/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_61.txt"
"/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_62.txt"
"/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_63.txt"
"/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_64.txt"
"/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_65.txt"
"/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_66.txt"
"/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_67.txt"
"/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_68.txt"
"/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_69.txt"
"/mnt/c/Users/samue/desktop/compiler/compiler_project2/pcode_test/pcode_70.txt"



    # Aggiungi altri script se necessario
)


  # Vettore dei valori di uscita attesi (esempio: success=0, failure=1, custom=42, etc.)
  expected_exit_codes=(0 0 0 0 0
                       0 0 0 0 139
                       0 0 0 0 0
                       0 0 0 0 0
                       0 0 0 0 0
                       0 0 0 0 0
                       0 0 0 0 0
                       0 0 0 0 0
                       0 0 0 0 0
                       0 0 0 0 0
                       0 0 0 0 0
                       0 0 0 0 0
                       0 0 0 0 0
                       0 0 0 0 0 )
  # Contatore per indicare l'indice corrente del vettore di valori attesi
# Inizializzazione delle variabili
index=0
PRINT_SUCCESS=false

# Controlla se i parametri per l'intervallo sono stati passati
if [[ "$1" == "-ver" ]]; then
    PRINT_SUCCESS=true
    shift  # Rimuovi -ver dai parametri per non interferire con gli altri
fi

# Verifica se sono stati passati gli estremi del ciclo (due numeri interi)
if [[ $# -ge 2 ]]; then
    START_INDEX=$1
    END_INDEX=$2
    shift 2  # Rimuove gli estremi passati dalla lista dei parametri
else
      START_INDEX=0
      END_INDEX=$((${#script_list[@]} - 1))
  fi

# Loop per eseguire ciascuno script Bash e successivamente il programma C
for ((index=$START_INDEX; index<=$END_INDEX; index++)); do
    script="${script_list[$index]}"
    # Verifica se lo script esiste ed è eseguibile
    if [[ -x "$script" ]]; then
      current_test_number=$((index+1))
     echo "Eseguendo lo script $current_test_number"
     echo " "
            # Ottieni l'exit status dell'ultima esecuzione del programma C
            if [ "$1" == "-last" ]; then
             script_output=$(bash "$script")
             fi

            C_PROGRAM_OPTIONS="/mnt/c/Users/samue/Desktop/compiler_project2/output/code.txt"  # Esempio di opzione per specificare l'output
            C_PROGRAM_OPTION_2=" -"  # Esempio di opzione per specificare l'output
            "$C_PROGRAM" "$script" # Esegui il programma C
            exit_status=$?
            # Verifica se l'exit status corrisponde a quello atteso
            if [[ $exit_status -eq ${expected_exit_codes[$index]} ]]; then
               if $PRINT_SUCCESS; then
                 echo "Eseguendo lo script $script"
                echo "\nSUCCESS: Exit status corretto ($exit_status)"

                fi
            else
              echo "Eseguendo lo script $script"
              echo "ERROR: Exit status errato! Atteso: ${expected_exit_codes[$index]}, Ricevuto: $exit_status"

            fi

            # Incrementa l'indice per passare al prossimo valore atteso (opzionale, se necessario)




    else
        echo "Eseguendo lo script $script"
        echo "Errore: Lo script $script non è eseguibile o non esiste."

    fi
    echo " "
    echo "------------------------"
done

echo "\nTutti gli script sono stati eseguiti."
