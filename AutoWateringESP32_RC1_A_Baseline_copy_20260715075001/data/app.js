async function refresh()
{
    try
    {
        const response = await fetch("/api/status");
        const data = await response.json();

        document.getElementById("connection").innerHTML =
            "🟢 Online";

        const now = new Date();

        document.getElementById("lastUpdate").innerHTML =
            "Letzte Aktualisierung: " +
            now.toLocaleTimeString("de-DE");

        const container =
            document.getElementById("pots");

        container.innerHTML = "";

        data.pots.forEach((pot,index)=>
        {
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

            let progressColor="#0d6efd";

            if(percent>=90)
                progressColor="#28a745";
            else if(percent>=50)
                progressColor="#ffc107";
            else
                progressColor="#0d6efd";

            if(pot.weight>pot.target)
                progressColor="#dc3545";

            container.innerHTML +=
            `
            <div class="card ${pot.stateClass}">

                <h2>Topf ${index+1}</h2>

                <div class="weight">
                    ${pot.weight.toFixed(1)} g
                </div>

                <div class="status">
                    ${statusText(pot)}
                </div>

                <hr>

                <div class="progress">

                    <div
                        class="progressbar"
                        style="
                            width:${percent}%;
                            background:${progressColor};
                        ">
                    </div>

                </div>

                <div class="percent">
                    ${percent.toFixed(0)} %
                </div>

                <p>
                    Start:
                    ${pot.start.toFixed(1)} g
                </p>

                <p>
                    Ziel:
                    ${pot.target.toFixed(1)} g
                </p>

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
    onclick="pumpPot(${index}, ${pot.pumpRunning})">

    ${pot.pumpRunning ? "⏹ Stop" : "▶ Start"}

</button>

                </div>

            </div>
            `;
        });
    }
    catch(e)
    {
        document.getElementById("connection").innerHTML =
            "🔴 Offline";

        document.getElementById("lastUpdate").innerHTML =
            "Keine Verbindung";
    }
}

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

async function resetPot(id)
{
    await fetch("/reset?pot="+id);
    refresh();
}

async function tarePot(id)
{
    await fetch("/tare?pot="+id);
    refresh();
}

async function pumpPot(id, running)
{
    if(running)
    {
        await fetch("/pumpoff?pot=" + id);
    }
    else
    {
        await fetch("/pumpon?pot=" + id);
    }

    refresh();
}

refresh();

setInterval(refresh,1000);