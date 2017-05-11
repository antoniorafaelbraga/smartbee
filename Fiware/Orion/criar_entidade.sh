#!/bin/bash
#Script tem que receber 5 parametros, senão não executará
if [ $# -lt 5 ]; then
   echo "./criar_entidade [id] [tipo] [temperatura] [umidade] [som] [co2]"
   exit 1
fi

(curl 35.184.233.148:1026/v1/updateContext -s -S --header 'Content-Type: application/json' \
    --header 'Accept: application/json' --header 'Fiware-Service: tcc' --header 'Fiware-ServicePath: /' -d @- | python -mjson.tool > resposta | grep "code") <<EOF
{
    "contextElements": [
        {
            "type": "$2",
            "isPattern": "false",
            "id": "$1",
            "attributes": [
                {
                    "name": "temperatura",
                    "type": "float",
                    "value": "$3"
                },
                {
                    "name": "umidade",
                    "type": "float",
                    "value": "$4"
                },
                {
                    "name": "data",
                    "type": "string",
                    "value": "26/04/2017"
                },
                {
                    "name": "hora",
                    "type": "string",
                    "value": "11:42:51"
                },
				{
                    "name": "som",
                    "type": "float",
                    "value": "$5"
                },
				{
                    "name": "dioxido",
                    "type": "float",
                    "value": "$6"
                }
            ]
        }
    ],
    "updateAction": "APPEND"
}
EOF
