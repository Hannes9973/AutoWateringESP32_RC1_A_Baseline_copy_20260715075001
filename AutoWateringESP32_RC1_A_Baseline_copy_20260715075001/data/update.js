async function uploadFirmware()
{
    const file =
        document.getElementById("firmware")
        .files[0];

    if(!file)
    {
        alert("Bitte Firmware auswählen.");
        return;
    }

    const form =
        new FormData();

    form.append("update", file);

    const xhr =
        new XMLHttpRequest();

    xhr.upload.onprogress =
        function(e)
        {
            if(e.lengthComputable)
            {
                let p =
                    Math.round(
                        e.loaded /
                        e.total *
                        100);

                document
                    .getElementById("progress")
                    .value = p;

                document
                    .getElementById("status")
                    .innerHTML =
                    "Upload " + p + "%";
            }
        };

    xhr.onload =
        function()
        {
            if(xhr.status == 200)
            {
                document
                    .getElementById("status")
                    .innerHTML =
                    "✅ Firmware erfolgreich. ESP startet neu...";
            }
            else
            {
                document
                    .getElementById("status")
                    .innerHTML =
                    "❌ Update fehlgeschlagen.";
            }
        };

    xhr.open(
        "POST",
        "/update/upload");

    xhr.send(form);
}