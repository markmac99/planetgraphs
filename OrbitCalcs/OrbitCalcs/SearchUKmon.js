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
	var res='<table><tr><td>Shower</td><td>Datestamp</td><td>Mag</td><td>Dir</td><td>Alt</td><td>Ra</td><td>Dec</td><td>Camera</td></tr>';
	var dta='';
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
		console.log(`${dta}`);
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
		document.getElementById("results").innerHTML = res;
		console.log(`${res}`);
	});
	var client2 = new AWS.S3({apiVersion: '2006-03-01'});
	var res2='<table><tr><td>Y</td><td>M</td><td>D</td><td>h</td><td>m</td><td>s</td><td>Mag</td><td>Dir</td><td>Alt</td><td>Ra</td><td>Dec</td><td>Camera</td></tr>';
	srchkey='consolidated/P_';
	dta='';
	var params2 = {
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

	params2.Key=srchkey.concat(yr).concat('-unified.csv');
	selp1='select Y, \"M\", D, h, \"m\", s, Mag, Az1, Alt1, Ra1, Dec1, ID ';
	selp1=selp1.concat('from s3object q where ');
	selp1=selp1.concat('cast(Y as int)=').concat(yr);
	selp1=selp1.concat(' and cast(\"M\" as int)=').concat(mt);
	selp1=selp1.concat(' and cast(D as int)=').concat(dy);
	selp1=selp1.concat(' and cast(h as int)=').concat(hr);
	selp1=selp1.concat(' and cast(\"m\" as int)=').concat(mn);
	console.log(`${selp1}`);
	params2.Expression= selp1;
	client2.selectObjectContent(params2, (err2, data2) => {
		if (err2) {
			switch (err2.name) {
			// Check against specific error codes that need custom handling
			}
			return;
		}

		// data.Payload is a Readable Stream
		const events2 = data2.Payload;
		for (const event2 of events2) {
			if (event2.Records) {
				// event.Records.Payload is a buffer containing
				// a single record, partial records, or multiple records
				dta=dta.concat(event2.Records.Payload.toString());
				dta=dta.concat(',');
			} else if (event2.Stats) {
				console.log(`Processed ${event2.Stats.Details.BytesProcessed} bytes`);
			} else if (event2.End) {
				console.log('SelectObjectContent completed');
			}
		}
		console.log(`${dta}`);
		dta=dta.replace(" ","");
		dta=dta.replace(/(\r\n|\n|\r)/gm,",");
		console.log(`${dta}`);
		flds=dta.split(",");
		var i;
		for (i=0;i<flds.length;i++)
		{
			if (i % 12 ==0 ) { res2=res2.concat('<tr>');}
			res2=res2.concat('<td>').concat(flds[i]).concat('</td>');
			if ((i+1) % 12 ==0 ) { res2=res2.concat('</tr>');}
		}
		res2=res2.concat('</tr></table>');
		document.getElementById("results2").innerHTML = res2;
		console.log(`${res2}`);
	});
}


// query for Pi data
// note all values are strings, left-padded with spaces.
// could convert data to ints for search with cast(Y as int) for example.

//select Y, "M", D, h, "m", s, Mag, Az1, Alt1, Ra1, Dec1, ID 
//from s3object q
//where Y='2020' and "M"=' 1' and D=' 3' and h='18' and "m"='29'