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
                       0 0 0 0 0 )
  # Contatore per indicare l'indice corrente del vettore di valori attesi
    index=0
PRINT_SUCCESS=false
if [[ "$1" == "-all" ]]; then
    PRINT_SUCCESS=true
fi
# Controlla se il programma C esiste
if [[ ! -x "$C_PROGRAM" ]]; then
    echo "Errore: Il programma C non è eseguibile o non esiste."
    exit 1
fi

# Loop per eseguire ciascuno script Bash e successivamente il programma C
for script in "${script_list[@]}"; do
    # Verifica se lo script esiste ed è eseguibile
    if [[ -x "$script" ]]; then

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
              echo "\nERROR: Exit status errato! Atteso: ${expected_exit_codes[$index]}, Ricevuto: $exit_status"

            fi

            # Incrementa l'indice per passare al prossimo valore atteso (opzionale, se necessario)
            ((index++))



    else
        echo "Eseguendo lo script $script"
        echo "Errore: Lo script $script non è eseguibile o non esiste."

    fi
    echo " "
    echo "------------------------"
done

echo "\nTutti gli script sono stati eseguiti."
