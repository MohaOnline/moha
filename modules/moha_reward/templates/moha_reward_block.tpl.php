<script src="https://js.braintreegateway.com/web/3.25.0/js/client.min.js"></script>
<script src="https://js.braintreegateway.com/web/3.25.0/js/paypal-checkout.min.js"></script>

<div class="moha-reward-block">
  <div>
  </div>
  <div class="moha-reward-button"></div>
</div>
<script>
  paypal.Button.render({
    braintree: braintree,
    client: {
      production: '<?php echo $contents[MOHA_REWARD__VARIABLE__PAYPAL_BRAINTREE_CLIENT_TOKEN]; ?>',
      sandbox: 'CLIENT_TOKEN_FROM_SERVER'
    },
    env: '<?php echo $contents[MOHA_REWARD__VARIABLE__PAYPAL_ENVIRONMENT]; ?>', // or 'sandbox'
</script>
