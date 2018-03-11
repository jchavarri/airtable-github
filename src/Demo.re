open Express;

let (>>) = (f, g, x) => x |> f |> g;

let raiseIfNone = message =>
  fun
  | Some(value) => value
  | None => failwith(message);

let apiKey =
  Js.Dict.get(Node.Process.process##env, "AIRTABLE_API_KEY")
  |> raiseIfNone("Airtable API key missing");

let dbId =
  Js.Dict.get(Node.Process.process##env, "AIRTABLE_DB_ID")
  |> raiseIfNone("Airtable database id missing");

let tableName =
  Js.Dict.get(Node.Process.process##env, "AIRTABLE_TABLE_NAME")
  |> raiseIfNone("Airtable database id missing");

let base = Airtable.(makeConfig(~apiKey, ()) |> airtable |> base(dbId));

let table = base(. tableName);

let update = status =>
  Airtable.update(
    "recsA2fRDg7u7UPN2",
    Js.Dict.fromArray([|("Status", status)|]),
    table,
  );

/* make a common JSON object representing success */
let makeSuccessJson = () => {
  let json = Js.Dict.empty();
  Js.Dict.set(json, "success", Js.Json.boolean(Js.true_));
  Js.Json.object_(json);
};

module Body = {
  let jsonDecoder = json =>
    Json.Decode.(
      {
        "action": json |> field("action", string) |> GitHub.action,
        "issue": json |> field("issue", GitHub.issue),
      }
    );
};

let app = express();

App.useOnPath(app, ~path="/") @@
Middleware.json(~limit=5.0 |> ByteLimit.mb, ());

App.get(app, ~path="/test") @@
Middleware.from((_next, _req) => Response.sendJson(makeSuccessJson()));

App.post(app, ~path="/postreceive") @@
Middleware.from(_next =>
  Request.bodyJSON
  >> raiseIfNone("Body is none")
  >> Body.jsonDecoder
  >> (
    decoded => {
      Js.log(decoded);
      switch (decoded##action) {
      | Reopened
      | Opened => update("Open")
      | Closed => update("Closed")
      | _ => ()
      };
      Response.sendJson(makeSuccessJson());
    }
  )
);

let onListen = (port, e) =>
  switch (e) {
  | exception (Js.Exn.Error(e)) =>
    Js.log(e);
    Node.Process.exit(1);
  | _ => Js.log @@ "Listening at http://127.0.0.1:" ++ string_of_int(port)
  };

App.listen(app, ~onListen=onListen(3000), ());