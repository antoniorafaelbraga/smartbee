#Comando SQL para o banco Mysql
#Cada vez que for adicionado mais uma entidade, tem que adicionar mais uma tabela com o seguinte nome <entityId_Colmeia>;
#São adicionados os seguintes campo
[recvTime
fiwareServicePath
entityId
entityType
data
data_md
hora
hora_md
som
som_md
temperatura
temperatura_md
umidade
umidade_md
monoxido
monoxido_md
amostra
]
  
CREATE TABLE IF NOT EXISTS `<NOME_COMEIA_Colmeia` (
  `amostra` integer primary key AUTO_INCREMENT,
  `temperatura` FLOAT NULL, `umidade` FLOAT NULL, 
  `som` FLOAT NULL, `data` VARCHAR(45) NULL,
  `dioxido` FLOAT NULL,
  `hora` VARCHAR(45) NULL, `entityId` VARCHAR(45) NOT NULL,  
  `fiwareServicePath` VARCHAR(45) NULL, `entityType` VARCHAR(45) NULL,   
  `recvTime` TEXT(45) NULL, `temperatura_md` TEXT(45) NULL,   
  `umidade_md` TEXT(45) NULL, `som_md` TEXT(45) NULL,
`data_md` TEXT(45) NULL, `hora_md` TEXT(45) NULL, `dioxido_md` TEXT(45) NULL);
