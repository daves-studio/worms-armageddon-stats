const uuid = require('uuid/v4');
const fs = require('fs');

module.exports = (logParser) => (req, res) => {
  if (Object.keys(req.files).length == 0) {
    return res.status(400).send('No files were uploaded.');
  }

  // The name of the input field (i.e. "sampleFile") is used to retrieve the uploaded file
  let uploadedFile = req.files.logFile;
  let uploadedFileName = uploadedFile.name;
  let uploadedFileNameExtension = uploadedFileName.split('.').pop();

  if(uploadedFileNameExtension !== "log") {
    return res.status(400).send('Invalid File');
  }

  return new Promise((resolve, reject) => {
    // Use the mv() method to place the file somewhere on your server
    const newFileName = `${uuid()}.log`
    const logFilePath = `./logs/uploaded/${newFileName}`;
    uploadedFile.mv(logFilePath, function(err) {
      if (err) {
        return reject(new Error(err))
      }

      return resolve(logFilePath)
    });
  })
    .then((logFilePath) => {
      return logParser(logFilePath, logFilePath.replace("uploaded","parsed"), uploadedFileName)
    })
    .then(() => {
      res.status(200).send('File uploaded!');
    })
    .catch((err) => {
      console.log(err)
      return res.status(500).send("Internal Server Error");
    })
}
