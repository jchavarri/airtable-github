exception DecodeError(string);

type action =
  | Assigned
  | Unassigned
  | Labeled
  | Unlabeled
  | Opened
  | Edited
  | Milestoned
  | Demilestoned
  | Closed
  | Reopened;

let action = action =>
  /* From https://developer.github.com/v3/activity/events/types/#issuesevent */
  switch (action) {
  | "assigned" => Assigned
  | "unassigned" => Unassigned
  | "labeled" => Labeled
  | "unlabeled" => Unlabeled
  | "opened" => Opened
  | "edited" => Edited
  | "milestoned" => Milestoned
  | "demilestoned" => Demilestoned
  | "closed" => Closed
  | "reopened" => Reopened
  | _ => raise @@ DecodeError("Expected valid 'action' value, got " ++ action)
  };

let issue = json =>
  Json.Decode.(
    {
      "url": json |> field("url", string),
      "number": json |> field("number", int),
      "title": json |> field("title", string),
    }
  );