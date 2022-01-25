<template>
  <div>
    <div>
      <h1>{{ title }}</h1>
    </div>

    <div>
      <div>
        <button @click="getLinkbotHardwareVersion()">Get Linkbot Hardware Version</button>
        <h2>{{ hardwareVersion }}</h2>
      </div>

      <div>
        <input v-model="userInputHardwareVersion" placeholder="Type Linkbot hardware version here..."/>
        <button @click="setLinkbotHardwareVersion()">Set Linkbot Hardware Version</button>
      </div>

      <div>{{ errorMessage }}</div>
    </div>

    <div>
    </div>
  </div>
</template>

<script>
// @ is an alias to /src
// import HelloWorld from '@/components/HelloWorld.vue'

export default {
  name: 'HardwareVersion',
  data() {
    return {
      title: 'Linkbot Hardware Version',
      hardwareVersion: '',
      userInputHardwareVersion: '',
      errorMessage: '',
    }
  },
  methods: {
    getLinkbotHardwareVersion() {
      this.errorMessage = ''
      let self = this
      var dongle = new window.RobotImpl2('LOCL')
      dongle.connect('ws://localhost:42000', 'LOCL').then(function() {
        dongle.proxy.readEeprom({'address': 0x430, 'size': 3}, function(result) {
          // check if result contains data
          let data = result.data
          console.log(data)

          if(data.length === 3) {
            self.hardwareVersion = data[0] + '.' + data[1] + '.' + data[2]
          } else {
            self.errorMessage = 'Error reading Linkbot hardware version'
          }
        })
      })
    },
    setLinkbotHardwareVersion() {
      var versionArray = this.userInputHardwareVersion.split('.')
      if(versionArray.length !== 3) {
        this.errorMessage('Invalid version: '+this.userInputHardwareVersion)
        return
      }

      let self = this
      var dongle = new window.RobotImpl2('LOCL')
      dongle.connect('ws://localhost:42000', 'LOCL').then(function() {
        dongle.proxy.writeEeprom({'address': 0x430, 'data': versionArray}, function(err, result) {
          if (err) {
            self.errorMessage = err
        } else {
          console.log(result)
          self.errorMessage = 'Done set hardware version!'
        }
      })
      })
    },
  }
}
</script>

