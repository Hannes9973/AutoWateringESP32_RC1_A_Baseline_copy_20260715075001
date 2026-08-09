//------------------------------------------------------------
// AutoWatering RC2.6.1
//------------------------------------------------------------

const container = document.getElementById("pots");

let dashboardBuilt = false;

//------------------------------------------------------------
// Daten holen
//------------------------------------------------------------

async function refresh()
{
    try
    {
        const response = await fetch("/api/status");
        const data = await response.json();

        document.getElementById("connection").innerHTML =
            "🟢 Online";

        document.getElementById("lastUpdate").innerHTML =
            "Letzte Aktualisierung: " +
            new Date().toLocaleTimeString("de-DE");

        //----------------------------------------------------

        if(!dashboardBuilt)
        {
            buildDashboard(data);
            dashboardBuilt = true;
        }

        updateDashboard(data);
        refreshEvents();
    }
    catch(e)
    {
        document.getElementById("connection").innerHTML =
            "🔴 Offline";

        document.getElementById("lastUpdate").innerHTML =
            "Keine Verbindung";
    }
}
async function refreshEvents()
{
    try
    {
        const response =
            await fetch("/api/events");

        const events =
            await response.json();

        const log =
            document.getElementById("eventLog");

        if(!log)
            return;

        log.innerHTML = "";

        events.slice().reverse().forEach(event =>
{
    log.innerHTML += event + "<br>";
});
    }
    catch(e)
    {
        const log =
            document.getElementById("eventLog");

        if(log)
            log.innerHTML =
                "Keine Daten";
    }
}
//------------------------------------------------------------
// Dashboard erzeugen
//------------------------------------------------------------

function buildDashboard(data)
{
    container.innerHTML = "";

    data.pots.forEach((pot,index)=>
    {
        container.innerHTML += `

<div
class="card ${pot.stateClass}"
id="card${index}">

<h2>Topf ${index+1}</h2>

<div
class="weight"
id="weight${index}">
</div>

<div
class="status"
id="status${index}">
</div>

<hr>

<div class="progress">

<div
class="progressbar"
id="progress${index}">
</div>

</div>

<div
class="percent"
id="percent${index}">
</div>

<div class="settings">

<label>Startgewicht</label>

<input
class="weightInput"
id="start${index}"
type="number"
step="0.1">

<label>Zielgewicht</label>

<input
class="weightInput"
id="target${index}"
type="number"
step="0.1">

<button
class="saveBtn"
onclick="savePot(${index})">

💾 Speichern

</button>

</div>

<div class="buttons">

<button
class="resetBtn"
onclick="resetPot(${index})">

Reset

</button>

<button
class="tareBtn"
onclick="tarePot(${index})">

Tara

</button>

<button
class="pumpBtn"
id="pumpBtn${index}"
onclick="pumpPot(${index})">

▶ Start

</button>

</div>

</div>

`;
    });
    container.innerHTML += `
<div class="card eventCard">

    <h2>📋 Systemprotokoll</h2>

    <div
        id="eventLog"
        class="eventLog">

        Lädt...

    </div>

</div>
`;
}

//------------------------------------------------------------
// Dashboard aktualisieren
//------------------------------------------------------------

function updateDashboard(data)
{
    data.pots.forEach((pot,index)=>
    {
        //----------------------------------------------------
        // Gewicht
        //----------------------------------------------------

        document.getElementById("weight"+index).innerHTML =
            pot.weight.toFixed(1) + " g";

        //----------------------------------------------------
        // Status
        //----------------------------------------------------

        document.getElementById("status"+index).innerHTML =
            statusText(pot);

        //----------------------------------------------------
        // Karte
        //----------------------------------------------------

        document.getElementById("card"+index).className =
            "card " + pot.stateClass;

        //----------------------------------------------------
        // Prozent
        //----------------------------------------------------

        let percent = 0;

        if(pot.target > pot.start)
        {
            percent =
                ((pot.weight-pot.start)/
                (pot.target-pot.start))*100;
        }

        percent =
            Math.max(0,
            Math.min(100,percent));

        let color="#0d6efd";

        if(percent>=90)
            color="#28a745";
        else if(percent>=50)
            color="#ffc107";

        if(pot.weight>pot.target)
            color="#dc3545";

        document.getElementById("progress"+index).style.width =
            percent+"%";

        document.getElementById("progress"+index).style.background =
            color;

        document.getElementById("percent"+index).innerHTML =
            percent.toFixed(0)+" %";

        //----------------------------------------------------
        // Eingabefelder NICHT überschreiben,
        // solange der Benutzer tippt
        //----------------------------------------------------

        if(document.activeElement.id != "start"+index)
        {
            document.getElementById("start"+index).value =
                pot.start.toFixed(1);
        }

        if(document.activeElement.id != "target"+index)
        {
            document.getElementById("target"+index).value =
                pot.target.toFixed(1);
        }

        //----------------------------------------------------
        // Pumpenbutton
        //----------------------------------------------------

        const btn =
            document.getElementById("pumpBtn"+index);

        if(pot.pumpRunning)
        {
            btn.innerHTML = "⏹ Stop";
        }
        else
        {
            btn.innerHTML = "▶ Start";
        }
    });
}
//------------------------------------------------------------
// Speichern
//------------------------------------------------------------

async function savePot(id)
{
    const start =
        document.getElementById("start"+id).value;

    const target =
        document.getElementById("target"+id).value;

    await fetch(
    "/save?pot="+id+
    "&start="+start+
    "&target="+target);

setTimeout(refresh,300);
}

//------------------------------------------------------------
// Reset
//------------------------------------------------------------

async function resetPot(id)
{
    await fetch("/reset?pot="+id);
}

//------------------------------------------------------------
// Tara
//------------------------------------------------------------

async function tarePot(id)
{
    await fetch("/tare?pot="+id);
}

//------------------------------------------------------------
// Pumpe
//------------------------------------------------------------

async function pumpPot(id)
{
    const button =
        document.getElementById("pumpBtn"+id);

    if(button.innerHTML.includes("Stop"))
    {
        await fetch("/pumpoff?pot="+id);
    }
    else
    {
        await fetch("/pumpon?pot="+id);
    }

    refresh();
}

//------------------------------------------------------------
// Statustext
//------------------------------------------------------------

function statusText(pot)
{
    switch(pot.stateClass)
    {
        case "idle":
            return "🟢 Bereit";

        case "wait":
            return "🟡 Warten";

        case "watering":
            return "💧 Gießen";

        case "error":
            return "🔴 Sensorfehler";

        default:
            return pot.state;
    }
}

//------------------------------------------------------------
// Start
//------------------------------------------------------------

refresh();

setInterval(refresh,1000);