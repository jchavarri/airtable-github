type connection;

type config;

type dbId = string;

type tableName = string;

type recordId = string;

type table;

type selectFilter;

type selectResult;

type record = {
  .
  "id": string,
  "fields": array(Js.Json.t),
};

[@bs.module] [@bs.new] external airtable : config => connection = "airtable";

[@bs.obj]
external makeConfig :
  (~apiKey: string=?, ~endpointUrl: string=?, ~requestTimeout: int=?, unit) =>
  config =
  "";

[@bs.send.pipe: connection]
external base : dbId => (. tableName) => table = "";

[@bs.obj]
external makeSelectFilter :
  (
    ~fields: array(string)=?,
    ~filterByFormula: string=?,
    ~maxRecords: int=?,
    ~pageSize: int=?,
    ~sort: array(Js.Dict.t('a))=?,
    ~view: string=?,
    ~cellFormat: string=?,
    ~timeZone: string=?,
    ~userLocale: string=?,
    unit
  ) =>
  selectFilter =
  "";

[@bs.send.pipe: table] external select : selectFilter => selectResult = "";

[@bs.send.pipe: selectResult]
external firstPage :
  (
    (
      . Js.Nullable.t({
          .
          error: int,
          message: string,
        }),
      array(record)
    ) =>
    unit
  ) =>
  unit =
  "";

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