function main()
{
    var testCase = {
        name: "New Score",
        steps: [
            {name: "Open Dialog", wait: false, func: function() {
                api.dispatcher.dispatch("file-new")
            }},
            {name: "Select Flute", func: function() {
                api.navigation.goToControl("NewScoreDialog", "FamilyView", "Woodwinds")
                api.navigation.goToControl("NewScoreDialog", "InstrumentsView", "Flute")

            }},
            {name: "Select Flute (apply)", func: function() {
                api.navigation.goToControl("NewScoreDialog", "SelectPanel", "Select")
                api.navigation.trigger()

            }},
            {name: "Select Piano", func: function() {
                api.navigation.goToControl("NewScoreDialog", "FamilyView", "Keyboards")
                api.navigation.goToControl("NewScoreDialog", "InstrumentsView", "Piano")


            }},
            {name: "Select Piano (apply)", func: function() {
                api.navigation.goToControl("NewScoreDialog", "SelectPanel", "Select")
                api.navigation.trigger()

            }},
            {name: "Done", func: function() {
                api.navigation.goToControl("NewScoreDialog", "BottomPanel", "Done")
                api.navigation.trigger()
            }},
        ]
    };

    api.autobot.setInterval(500)
    api.autobot.runTestCase(testCase)
    api.log.info("----------- end script ---------------")
}