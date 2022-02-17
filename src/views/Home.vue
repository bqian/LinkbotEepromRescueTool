<template>
  <div class="home">
    <EepromView name="Serial Id" addr=0x412 size=4 type="char"/>
    <EepromView name="Hardware Version" addr=0x430 size=3 type="byte"/>
    <EepromView name="Motor 1 Calibration Value" addr=0x00 size=4 type="float"/>
    <EepromView name="Motor 2 Calibration Value" addr=0x04 size=4 type="float"/>
    <EepromView name="Motor 3 Calibration Value" addr=0x08 size=4 type="float"/>
    <EepromView name="Motor Calibrated Number" addr=0x10 size=1 type="byte"/>
    <EepromView name="Motor Second Calibrated Number" addr=0x416 size=1 type="byte"/>
    <EepromView name="Motor 1 Max encoder voltage" addr=0x417 size=4 type="float"/>
    <EepromView name="Motor 2 Max encoder voltage" addr=0x41B size=4 type="float"/>
    <EepromView name="Motor 3 Max encoder voltage" addr=0x41F size=4 type="float"/>
  </div>
</template>

<script>
// @ is an alias to /src
import EepromView from '@/components/EepromView.vue'

export default {
  name: 'Home',
  components: {
    EepromView,
  }
}
</script>
