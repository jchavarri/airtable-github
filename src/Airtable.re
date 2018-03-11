type connection;

type config;

type dbId = string;

type tableName = string;

type recordId = string;

type table;

[@bs.module] [@bs.new] external airtable : config => connection = "airtable";

[@bs.obj]
external makeConfig :
  (~apiKey: string=?, ~endpointUrl: string=?, ~requestTimeout: int=?, unit) =>
  config =
  "";

[@bs.send.pipe: connection]
external base : dbId => (. tableName) => table = "";

[@bs.send.pipe: table]
external update : (recordId, Js.Dict.t('a)) => unit = "";
/* Usage

   let base =
     Airtable.(makeConfig(~apiKey, ()) |> airtable |> base(dbId));

   let table = base(. tableName);

   Airtable.update(
     recordId,
     Js.Dict.fromArray([|("InventoryItems", 3)|]),
     table
   );
    */