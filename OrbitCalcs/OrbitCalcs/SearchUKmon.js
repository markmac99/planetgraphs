//    <script type="text/javascript">

        // Initialize the Amazon Cognito credentials provider
        AWS.config.region = 'eu-west-2'; 
        AWS.config.credentials = new AWS.CognitoIdentityCredentials({
             IdentityPoolId: 'eu-west-2:2cf7d5c1-9510-4aad-8a04-4d4effe8d1d7'});
        
     function queryS3()
     {
      var yr=document.getElementById("YearEntry").value;
      var mt=document.getElementById("MthEntry").value;
      var dy=document.getElementById("DayEntry").value;
      var hr=document.getElementById("HrEntry").value;
      var mn=document.getElementById("MinEntry").value;
      
      var srchval=yr.concat(mt).concat(dy).concat('_').concat(hr).concat(mn).concat('%');
      var srchkey='consolidated/M_';
      var selp1= 'SELECT \"Group\",LocalTime,Mag,Dir1,Alt1,Ra1,Dec1,Loc_Cam FROM S3Object WHERE LocalTime like \'';
      
      var client = new AWS.S3({apiVersion: '2006-03-01'});
      var params = {
	Bucket: 'ukmon-shared',
	Key: '',
	ExpressionType: 'SQL',
	Expression: '',
	InputSerialization: {
		CSV: {
			FileHeaderInfo: 'USE',
			RecordDelimiter: '\n',
			FieldDelimiter: ','
		}
	},
	OutputSerialization: {
		CSV: {}
	}
      };
      params.Expression= selp1.concat(srchval).concat('\'');
      params.Key=srchkey.concat(yr).concat('-unified.csv');
      client.selectObjectContent(params, (err, data) => {
	if (err) {
		switch (err.name) {
			// Check against specific error codes that need custom handling
		}
		return;
	}

	// data.Payload is a Readable Stream
	const events = data.Payload;
	var res='<table><tr><td>Shower</td><td>Datestamp</td><td>Mag</td><td>Dir</td><td>Alt</td><td>Ra</td><td>Dec</td><td>Camera</td></tr>';
	var dta='';
	for (const event of events) {
		if (event.Records) {
			// event.Records.Payload is a buffer containing
			// a single record, partial records, or multiple records
			dta=dta.concat(event.Records.Payload.toString());
			dta=dta.concat(',');
		} else if (event.Stats) {
			console.log(`Processed ${event.Stats.Details.BytesProcessed} bytes`);
		} else if (event.End) {
			console.log('SelectObjectContent completed');
		}
	}
	//console.log(`${dta}`);
	dta=dta.replace(" ","");
	dta=dta.replace(/(\r\n|\n|\r)/gm,",");
	console.log(`${dta}`);
	flds=dta.split(",");
	var i;
 	for (i=0;i<flds.length;i++)
	{
	  if (i % 8 ==0 ) { res=res.concat('<tr>');}
	  res=res.concat('<td>').concat(flds[i]).concat('</td>');
	  if ((i+1) % 8 ==0 ) { res=res.concat('</tr>');}
	}
        res=res.concat('</tr></table>');
        //console.log(`${res}`);
        document.getElementById("results").innerHTML = res;
      });
    }
//    </script>
