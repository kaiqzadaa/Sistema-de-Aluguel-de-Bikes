/*
							CONTACT OF DEVELOPER
					Github: https://github.com/kaiqzadaa
					Discord: Kaiqzadaa

*/

#include <YSI_Coding\y_hooks>

/*
    Definers
*/

#define MAX_BIKES (30)

/*
    Enum
*/

enum E_BIKE_PLAYER {
    bikeModel,

    Float:bikePosX,
    Float:bikePosY,
    Float:bikePosZ,
    Float:bikePosA,

    bool:bikeSpawn
};

static bikePlayer[MAX_PLAYERS][E_BIKE_PLAYER];

enum E_BIKE_DATA {
    bikeID,
    Text3D:bikeLabel,
    bikePickup,

    Float:bikePosX,
    Float:bikePosY,
    Float:bikePosZ,
    Float:bikePosA,

    bool:bikeExists,
}

static bikeData[MAX_BIKES][E_BIKE_DATA];

/*
    Hooks
*/

hook OnGameModeInit() {
    new Cache:connect;
    connect = mysql_query(lgDatabaseHandle, "SELECT * FROM `Bikes`");

    if(lgDatabaseHandle == MYSQL_INVALID_HANDLE || mysql_errno(lgDatabaseHandle) != 0) {
        SendRconCommand("exit");
        print("[open.mp hashbury:bike's]: Nao foi possivel concluir a conexao ao banco de dados das bikes");
        return true;
    }

    print("[open.mp hashbury:bike's]: Sistema de bikes esta sendo carregado.");

    for(new index; index < MAX_BIKES; index++)
        BIKE_Load(index);

    print("[open.mp hashbury:bike's]: Conexao ao banco de dados das bikes estabelecida com sucesso");
    cache_delete(connect);

    return true;
}

hook OnPlayerDisconnect(playerid, reason) {
    bikePlayer[playerid][bikeSpawn] = false;

    return true;
}

hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys) {
    if(PRESSED(KEY_YES)) {
        new bikeid;

        if((bikeid = BIKE_Nearby(playerid)) != -1)
            BIKE_Show(playerid, bikeid);

    }

    return true;
}

/*
    Commands
*/

CMD:createbike(playerid) {
    new bikeid;

    if((bikeid = BIKE_Create(playerid)) == -1)
        return SendClientMessage(playerid, -1, "Limites de bike's criados atingido");

    //Salva
    SendClientMessage(playerid, -1, "Voce criou o BIKE %i", bikeid);

    return true;
}

CMD:removebike(playerid, params[]) {
    new bikeid;

    if(sscanf(params, "i", bikeid))
        return SendClientMessage(playerid, -1, "Utilize o comando /removebike [bikeID]");

    if(!bikeData[bikeid][bikeExists])
        return SendClientMessage(playerid, -1, "O BIKE %i nao existe!", bikeid);

    BIKE_Remove(bikeid);
    SendClientMessage(playerid, -1, "Voce removeu o BIKE %i", bikeid);

    return true;
}

/*
    Native 
*/

BIKE_Create(playerid) {
    new bikeid;

    if((bikeid = BIKE_Count()) != -1) {
        static
            Float:x,
            Float:y,
            Float:z,
            Float:angle
        ;

        GetPlayerPos(playerid, x, y, z);
        GetPlayerFacingAngle(playerid, angle);

        bikeData[bikeid][bikeExists] = true;
        bikeData[bikeid][bikeID] = bikeid;

        bikeData[bikeid][bikePosX] = x;
        bikeData[bikeid][bikePosY] = y;
        bikeData[bikeid][bikePosZ] = z;
        bikeData[bikeid][bikePosA] = angle;        

        BIKE_Save(bikeid);

        BIKE_Refresh(bikeid);
        return bikeid;
    }

    return -1;
}

BIKE_Count() {
    new slotIdx = -1;

    for(new index; index < MAX_BIKES; index++) {
        if(bikeData[index][bikeExists])
            continue;

        slotIdx = index;
        break;
    }

    return slotIdx;
}

BIKE_Remove(bikeid) {
    if(bikeData[bikeid][bikeExists]) {
        //Executa a query

        DestroyDynamic3DTextLabel(bikeData[bikeid][bikeLabel]);
        DestroyDynamicPickup(bikeData[bikeid][bikePickup]);

        new resetBIKE[E_BIKE_DATA];
        bikeData[bikeid] = resetBIKE;

        BIKE_Destroy(bikeid);
    }

    return true;
}

BIKE_Refresh(bikeid) {
    bikeData[bikeid][bikePickup] = CreateDynamicPickup(1239, 1, bikeData[bikeid][bikePosX], bikeData[bikeid][bikePosY], bikeData[bikeid][bikePosZ], 0, 0, -1, 90.0, -1);
    bikeData[bikeid][bikeLabel] = CreateDynamic3DTextLabel("{7672E5}Itaú bikes\n{FFFFFF}Pressione '{7672E5}Y{FFFFFF}' para interagir", -1, bikeData[bikeid][bikePosX], bikeData[bikeid][bikePosY], bikeData[bikeid][bikePosZ], 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, 0, -1, 13.0, -1);
    return true;
}

BIKE_Load(index) {
    new rows = cache_num_rows();

    if(rows > 0) {
        cache_get_value_name_int(0, "BikeID", index);
        bikeData[index][bikeID] = index;

        cache_get_value_name_float(0, "BikePosX", bikeData[index][bikePosX]);
        cache_get_value_name_float(0, "BikePosY", bikeData[index][bikePosY]);
        cache_get_value_name_float(0, "BikePosZ", bikeData[index][bikePosZ]);
        cache_get_value_name_float(0, "BikePosA", bikeData[index][bikePosA]);

        bikeData[index][bikeExists] = true;

        BIKE_Refresh(index);
    }

    return true;
}

BIKE_Save(bikeid) {
    mysql_format(lgDatabaseHandle, query, sizeof(query), "INSERT INTO `Bikes` (`BikeID`, `BikePosX`, `BikePosY`, `BikePosZ`, `BikePosA`) VALUES ('%d', '%f', '%f', '%f', '%f');", bikeid, bikeData[bikeid][bikePosX], bikeData[bikeid][bikePosY], bikeData[bikeid][bikePosZ], bikeData[bikeid][bikePosA]);
    mysql_tquery(lgDatabaseHandle, query);

    if(lgDatabaseHandle == MYSQL_INVALID_HANDLE || mysql_errno(lgDatabaseHandle) != 0) 
        return print("[open.mp System:bike's]: Houve um problema ao adicionar esse ponto");

    return true;
}

BIKE_Destroy(bikeid) {
    mysql_format(lgDatabaseHandle, query, sizeof(query), "DELETE FROM `Bikes` WHERE `BikeID` = %d", bikeid);
    mysql_tquery(lgDatabaseHandle, query);

    if(lgDatabaseHandle == MYSQL_INVALID_HANDLE || mysql_errno(lgDatabaseHandle) != 0) 
        return print("[open.mp System:bike's]: Houve um problema ao remover esse ponto");

    return true;
}

BIKE_Show(playerid, bikeid) {
    if (bikePlayer[playerid][bikeSpawn] == false) {
        bikePlayer[playerid][bikeModel] = CreateVehicle(481, bikeData[bikeid][bikePosX], bikeData[bikeid][bikePosY], bikeData[bikeid][bikePosZ], bikeData[bikeid][bikePosA], 186, 186, 0);
        PutPlayerInVehicle(playerid, bikePlayer[playerid][bikeModel], 0);
        bikePlayer[playerid][bikeSpawn] = true;
        SendClientMessage(playerid, -1, "Sucesso! voce pegou sua bike");
    } else {
        DestroyVehicle(bikePlayer[playerid][bikeModel]);
        bikePlayer[playerid][bikeSpawn] = false;
        SendClientMessage(playerid, -1, "Voce ja tem uma bike");
    }
}

BIKE_Nearby(playerid, Float:radius = 10.0) {
    for(new index; index < MAX_BIKES; index++) {
        if(bikeData[index][bikeExists]) {
            if(IsPlayerInRangeOfPoint(playerid, radius, bikeData[index][bikePosX], bikeData[index][bikePosY], bikeData[index][bikePosZ]))
                return index;
        }
    }

    return -1;
}
