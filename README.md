# Github + Airtable integration

Hello! This project, written in [Reason](reasonml.github.io), explores some basic integration of GitHub issues with Airtable (it updates a field whenever an issue is opened or closed).

It contains:

- A very simple GitHub webhook to handle events from issues (note it will probably throw with other types of events)
- Some functions to update data in Airtable

# Build
```
npm run build
```

# Build + Watch

```
npm run start
```

# Run 

```
AIRTABLE_API_KEY="your_api_key" AIRTABLE_DB_ID="your_db_id" AIRTABLE_TABLE_NAME="your_table_name" node src/Main.bs.js
```