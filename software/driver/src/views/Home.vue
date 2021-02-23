<template>
  <div class="home">
    <img alt="Vue logo" src="../assets/logo.png" />
    {{ appName }}
  </div>
</template>

<script lang="ts">
import { Component, Vue } from "vue-property-decorator";
import electron from "electron";

@Component
export default class Home extends Vue {
  appName = "Penis";
  constructor() {
    super();
    this.appName = electron.remote.app.getName();
  }
}
</script>
