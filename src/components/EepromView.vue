<template>
  <n-card
    :title="name"
    :segmented="{
      content: true,
      footer: 'soft'
    }"
  >
    Address: {{ addr }}
    size: {{ size }}
    type: {{ type }}
  </n-card>
</template>

<style>
.n-card {
  max-width: 300px;
}
</style>

<script>
export default {
  name: 'EepromView',
  props: {
    name: String,
    addr: Number,
    size: Number,
    type: String,
  },
  data() {
    return {
      bytes: [],
    }
  },
  methods: {
    getEepromData() {
      //this.errorMessage = ''
      let self = this
      var dongle = new window.RobotImpl2('LOCL')
      dongle.connect('ws://localhost:42000', 'LOCL').then(function() {
        dongle.proxy.readEeprom({'address': self.addr, 'size': self.size}, function(result) {
          // check if result contains data
          let data = result.data
          console.log(data)

          if(data.length === self.size) {
            self.bytes = data
          } else {
            //self.errorMessage = 'Error reading Linkbot hardware version'
          }
        })
      })
    },
  },
  compuated: {
    generateTitle() {
      return this.name+ ': ' + this.bytes
    },
  },
  mounted() {
    this.getEepromData()
  },
}
</script>
